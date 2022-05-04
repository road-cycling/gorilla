#include "blockbuilder.hpp"

#include <iostream>
#include <bitset>

#define _0    0
#define _01   1   // 10
#define _011  3   // 110
#define _0111 7   // 1110
#define _1111 15  // 1111

BlockBuilder::BlockBuilder(){}
BlockBuilder::~BlockBuilder(){}
void BlockBuilder::Serialize(){}

std::vector<std::pair<int, double>> BlockBuilder::ReadOutData() {

    std::vector<std::pair<int, double>> decompressedData{};
    decompressedData.resize(this->pointsWritten);

    for (int i = 0; i < this->pointsWritten; i++) {
        decompressedData.emplace_back(this->ReadPoint(), this->ReadDouble());
    }

    return decompressedData;

}

double BlockBuilder::ReadDouble() {

    if ( this->firstDoubleRead == false ) {

        #ifdef DEBUG
            std::cout << "Reading First Double From Stream" << std::endl;
        #endif

        this->firstDoubleRead = true;
        uint64_t readOutFirstValueU64 = 0;
        
        this->bitStream->BitReader64(readOutFirstValueU64, 64);
        double readOutFirstValue = 0; 

        std::memcpy(&readOutFirstValue, &readOutFirstValueU64, 8);
        this->lastDoubleRead = readOutFirstValueU64;

        return readOutFirstValue;
    }

    int encodedBit = 0;
    this->bitStream->BitReader(encodedBit, 1);

    // XOR with the previous is 0. Last value read out can be returned
    if ( encodedBit == 0 ) {

        #ifdef DEBUG
            std::cout << "Last value stored is same as next ~" << std::endl;
        #endif

        double readOutFirstValue = 0;
        std::memcpy(&readOutFirstValue, &this->lastDoubleRead, 8);
        return readOutFirstValue;
    }

    int controlBit = 0;
    this->bitStream->BitReader(controlBit, 1);

    // Control Bit '0'
    if ( controlBit == 0 ) {

        // If the block of meaningful bits falls within the
        // block of previous meaningful bits
        int bitsToRead = 64 - this->lastReadTrailingZeroes - this->lastReadLeadingZeroes;
        uint64_t readOutXOR = 0;
        this->bitStream->BitReader64(readOutXOR, bitsToRead);
        readOutXOR <<= this->lastReadTrailingZeroes;

        readOutXOR ^= this->lastDoubleRead;
        this->lastDoubleRead = readOutXOR;

        #ifdef DEBUG
            std::cout << "Control bit='0' Read out XOR =" << readOutXOR  << std::endl;
        #endif

        double readBackDouble0 = 0;
        std::memcpy(&readBackDouble0, &readOutXOR, 8);
        return readBackDouble0;
    }


    // Control Bit '1'
    else if ( controlBit == 1 ) {
        int leadingZeroes = 0;
        int lengthMeaningfulXORedValue = 0;
        uint64_t meaningfulXORdValue = 0;

        // leading zeroes in the next 5 bits
        this->bitStream->BitReader(leadingZeroes, 5);

        // length of XORed value in next 6 bits
        this->bitStream->BitReader(lengthMeaningfulXORedValue, 6);

        // meaningful bits of the XORed value
        this->bitStream->BitReader64(meaningfulXORdValue, lengthMeaningfulXORedValue);

        this->lastReadLeadingZeroes = leadingZeroes;
        this->lastReadTrailingZeroes = 64 - leadingZeroes - lengthMeaningfulXORedValue;

        meaningfulXORdValue <<= this->lastReadTrailingZeroes;

        meaningfulXORdValue ^= this->lastDoubleRead;
        this->lastDoubleRead = meaningfulXORdValue;

        #ifdef DEBUG
            std::cout << "Control bit='1' Leading 0=" << leadingZeroes << " Len XOR=" << lengthMeaningfulXORedValue << " Meaningful XOR=" << meaningfulXORdValue << std::endl;
        #endif


        double readBackDouble = 0;
        std::memcpy(&readBackDouble, &this->lastDoubleRead, 8);
        return readBackDouble;
    }

    return 0.;

}

void BlockBuilder::WriteDouble(double dataValue) {

    uint64_t dataValueAsInt = 0;
    std::memcpy(&dataValueAsInt, &dataValue, 8);

    uint64_t dataValueToWriteToStream = 0;

    if ( this->doubleHasBeenWritten == false ) {
        this->doubleHasBeenWritten = true;
        dataValueToWriteToStream = dataValueAsInt;
        previousDoubleWritten = dataValueToWriteToStream;

        // 1. The first value is stored with no compression.
        return this->bitStream->WriteBits(dataValueToWriteToStream, 64);

    } else {
        dataValueToWriteToStream = dataValueAsInt ^ this->previousDoubleWritten;
        this->previousDoubleWritten = dataValueAsInt;
    }

#ifdef DEBUG
    std::cout << "Writing: " << std::hex << dataValueToWriteToStream << std::endl;
#endif

    // 2. if XOR with the previous is zero (same value)
    // store a single 0 bit
    if (dataValueToWriteToStream == 0) {
        return this->bitStream->WriteBits(0, 1);
    }

    int leadingZeroes = std::countl_zero(dataValueToWriteToStream);
    int trailingZeroes = std::countr_zero(dataValueToWriteToStream);

#ifdef DEBUG
    std::cout << "Leading Z: " << leadingZeroes << std::endl;
    std::cout << "Trailing Z: " << trailingZeroes << std::endl;
    std::cout << std::endl;
#endif

    // 3. When XOR is non=zero, calculate the number of leading and trailing zeroes
    // in the XOR.

    // Store bit '1'
    this->bitStream->WriteBits(1, 1);

    // Followed by either
    if (leadingZeroes == this->previouslyWrittenLeadingZeroes && trailingZeroes == this->previouslyWrittenTrailingZeroes) {
        // a) (Control bit '0') If the block of meaningful bits falls within the block of previous meaningful bits
        this->bitStream->WriteBits(0, 1);
        int lengthMeaningfulXOR = 64 - leadingZeroes - trailingZeroes;
        // Right align.
        dataValueToWriteToStream >>= trailingZeroes;
        this->bitStream->WriteBits(dataValueToWriteToStream, lengthMeaningfulXOR);
    } else {

        // b) (Control bit '1')
        //       - Store the length of the number of leading zeroes in the next 5 bits
        //       - Store the length of the meaningful XORed value in the next 6 bits
        //       - Store the meaningful bits of the XORed value
        
        // (1) Control Bit
        this->bitStream->WriteBits(1, 1);

        // (5) Length of the number of leading zeroes
        this->bitStream->WriteBits(leadingZeroes, 5);

        // (6) Length of the meaningful XORed value
        int lengthMeaningfulXOR = 64 - leadingZeroes - trailingZeroes;
        this->bitStream->WriteBits(lengthMeaningfulXOR, 6);

        // Store the meaningful bits of the XORed value

        // Right align.
        dataValueToWriteToStream >>= trailingZeroes;
        this->bitStream->WriteBits(dataValueToWriteToStream, lengthMeaningfulXOR);

        this->previouslyWrittenTrailingZeroes = trailingZeroes;
        this->previouslyWrittenLeadingZeroes = leadingZeroes;

    }
   
}

void BlockBuilder::WritePoint(int timestamp) {

    if (this->previousTimestamp == -1) {
        this->previousTimestamp = (timestamp / (60 * 60 * 2)) * (60 * 60 * 2);
        this->blockStart = this->previousTimestamp;
    }

    int delta = timestamp - this->previousTimestamp;
    int deltaOfDelta = delta - this->lastDelta;

#ifdef DEBUG
    std::cout << "Δ: " << delta << "\t Δ: " << deltaOfDelta << std::endl;
#endif

    if (deltaOfDelta == 0) {
        this->bitStream->WriteBits(_0, 1);
    } else if (-63 <= deltaOfDelta && deltaOfDelta <= 64) {
        this->bitStream->WriteBits(_01, 2);
        BitStream::WriteToFlipPotentialNegative(deltaOfDelta, 7);
        this->bitStream->WriteBits(deltaOfDelta, 7);
    } else if (-255 <= deltaOfDelta && deltaOfDelta <= 256) {
        this->bitStream->WriteBits(_011, 3);
        BitStream::WriteToFlipPotentialNegative(deltaOfDelta, 9);
        this->bitStream->WriteBits(deltaOfDelta, 9);
    } else if (-2047 <= deltaOfDelta && deltaOfDelta <= 2048) {
        this->bitStream->WriteBits(_0111, 4);
        BitStream::WriteToFlipPotentialNegative(deltaOfDelta, 12);
        this->bitStream->WriteBits(deltaOfDelta, 12);
    } else {
        this->bitStream->WriteBits(_1111, 4);
        this->bitStream->WriteBits(deltaOfDelta, 32);
    }

    this->lastDelta = delta;
    this->previousTimestamp = timestamp;
    this->pointsWritten += 1;

}

int BlockBuilder::ReadPoint() {

    int delta = 0;
    if ( this->timestampHasBeenReadFirstTime == false ) {
        this->timestampHasBeenReadFirstTime = true;
        this->lastReadTimestamp = this->blockStart;
    }

    int firstBitFromStream = 0;
    this->bitStream->BitReader(firstBitFromStream, 1);

    // (b) If D is zero, then store a single ‘0’ bit
    if ( firstBitFromStream == 0 ) {
        #ifdef DEBUG
                std::cout << "1Δ=0" << std::endl;
        #endif
        this->lastReadTimestamp += this->trailingDelta;
        return this->lastReadTimestamp;
    }

    firstBitFromStream = 0;
    this->bitStream->BitReader(firstBitFromStream, 1);

    // If D is between [-63, 64], store ‘10’ 
    // followed by the value (7 bits)
    // (previous was 1 now a 0)
    if ( firstBitFromStream == 0 ) {
        
        this->bitStream->BitReader(delta, 7);

        if (delta > 64) {
            delta ^= (1 << 6);
            delta *= -1;
        }

        #ifdef DEBUG
            std::cout << "2Δ=" << delta << std::endl;
        #endif

        this->lastReadTimestamp += (delta + this->trailingDelta);
        this->trailingDelta += delta;
        
        return this->lastReadTimestamp;

    }

    firstBitFromStream = 0;
    this->bitStream->BitReader(firstBitFromStream, 1);

    // If D is between [-255, 256], store 
    // ‘110’ followed by the value (9 bits)
    if (firstBitFromStream == 0) {
        
        this->bitStream->BitReader(delta, 9);

        if (delta > 256) {
            delta ^= (1 << 8);
            delta *= -1;
        }

        #ifdef DEBUG
            std::cout << "3Δ=" << delta << std::endl;
        #endif

        this->lastReadTimestamp += (delta + this->trailingDelta);
        this->trailingDelta += delta;
        return this->lastReadTimestamp;
    }

    // if D is between [-2047, 2048], store ‘1110’ 
    // followed by the value (12 bits)
    if (firstBitFromStream == 0) {
        this->bitStream->BitReader(delta, 12);

        if (delta > 2048) {
            delta ^= (1 << 11);
            delta *= -1;
        }

        #ifdef DEBUG
            std::cout << "4Δ=" << delta << std::endl;
        #endif

        this->lastReadTimestamp += (delta + this->trailingDelta);
        this->trailingDelta += delta;
        return this->lastReadTimestamp;
    }

    // Otherwise store ‘1111’ followed by D using 32 bits
    this->bitStream->BitReader(delta, 32);

    #ifdef DEBUG
        std::cout << "4Δ=" << delta << std::endl;   
    #endif

    this->lastReadTimestamp += (delta + this->trailingDelta);
    this->trailingDelta += delta;

    return this->lastReadTimestamp;
}

std::vector<int> BlockBuilder::ReadBackPoints() {

    std::vector<int> decompressedTimeSeries;

    for (int i = 0; i < this->pointsWritten; i++) {
        decompressedTimeSeries.push_back(this->ReadPoint());
    }

    return decompressedTimeSeries;

}

void BitStream::WriteToFlipPotentialNegative(int &inputInt, int numberOfBits) {
    if (0 > inputInt) {
        inputInt *= -1;
        inputInt |= (1<<(numberOfBits - 1));
    }
}

std::vector<std::bitset<64>> BlockBuilder::ReadBackBitstream() {

    auto readout = std::vector<std::bitset<64>>();
    for (auto bitstreamPack : *this->bitStream->byteStream) {
        readout.push_back(std::bitset<64>(bitstreamPack));
    }

    return readout;

}