#include "bitstream.hpp"

#include <bitset>
#include <iostream>


BitStream::BitStream(){
    this->byteStream->push_back(0);
}

BitStream::~BitStream(){}

void BitStream::print() {
    for (int i = 0; i < this->byteStream->size(); i++) {
        std::bitset<64> b(this->byteStream->at(i));
        std::cout << b << std::endl;
    }
}

void BitStream::bitWriter(uint_fast64_t writeInt, int numberOfBits) {
    this->byteStream->back() |= (writeInt << this->writeByteStreamStartingOffset);
    this->writeByteStreamStartingOffset += numberOfBits;
}

void BitStream::WriteBits(uint64_t writeInt, int numberOfBits) {

int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableInCurrentValue = bitsOnStreamBlock - this->writeByteStreamStartingOffset;
    bool streamOverflow = numberOfBits > bitsAvailableInCurrentValue;

    if (numberOfBits > bitsAvailableInCurrentValue) {

        int bitsThatCanBeWrittenToThisBlock = bitsAvailableInCurrentValue;
        int bitsToWriteToNextBlock = numberOfBits - bitsAvailableInCurrentValue;

        uint_fast64_t writeIntCopyForNextBlock = writeInt;
        uint_fast64_t writeIntCopy = (uint_fast64_t) writeInt;

        int thisBlockBitShift = bitsOnStreamBlock - bitsThatCanBeWrittenToThisBlock;
        writeIntCopy <<= thisBlockBitShift;
        writeIntCopy >>= thisBlockBitShift;

        writeIntCopyForNextBlock >>= bitsThatCanBeWrittenToThisBlock;

        this->bitWriter(writeIntCopy, bitsThatCanBeWrittenToThisBlock);
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
        this->bitWriter(writeIntCopyForNextBlock, bitsToWriteToNextBlock);

        return;
    }


    this->bitWriter(writeInt, numberOfBits);

    if (this->writeByteStreamStartingOffset == 64) {
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
    }

}


void BitStream::WriteBits(int writeInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableInCurrentValue = bitsOnStreamBlock - this->writeByteStreamStartingOffset;
    bool streamOverflow = numberOfBits > bitsAvailableInCurrentValue;

    if (numberOfBits > bitsAvailableInCurrentValue) {

        int bitsThatCanBeWrittenToThisBlock = bitsAvailableInCurrentValue;
        int bitsToWriteToNextBlock = numberOfBits - bitsAvailableInCurrentValue;

        uint_fast64_t writeIntCopyForNextBlock = writeInt;
        uint_fast64_t writeIntCopy = (uint_fast64_t) writeInt;

        int thisBlockBitShift = bitsOnStreamBlock - bitsThatCanBeWrittenToThisBlock;
        writeIntCopy <<= thisBlockBitShift;
        writeIntCopy >>= thisBlockBitShift;

        writeIntCopyForNextBlock >>= bitsThatCanBeWrittenToThisBlock;

        this->bitWriter(writeIntCopy, bitsThatCanBeWrittenToThisBlock);
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
        this->bitWriter(writeIntCopyForNextBlock, bitsToWriteToNextBlock);

        return;
    }


    this->bitWriter(writeInt, numberOfBits);

    if (this->writeByteStreamStartingOffset == 64) {
        this->byteStream->push_back(0);
        this->writeByteStreamStartingOffset = 0;
    }
    
}

void BitStream::bitReader(int &readInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;

    uint_fast64_t extractor = 0;
    extractor |= this->byteStream->at(this->readByteStreamVectorOffset);

    extractor <<= (bitsOnStreamBlock - (this->readByteStreamBlockOffset + numberOfBits));
    extractor >>= (bitsOnStreamBlock - numberOfBits);


    readInt = extractor;
    this->readByteStreamBlockOffset += numberOfBits;
    if (this->readByteStreamBlockOffset == 64) {
        this->readByteStreamBlockOffset = 0;
        this->readByteStreamVectorOffset += 1;
    }
}

void BitStream::bitReader64( uint64_t &readInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;

    uint_fast64_t extractor = 0;
    extractor |= this->byteStream->at(this->readByteStreamVectorOffset);

    extractor <<= (bitsOnStreamBlock - (this->readByteStreamBlockOffset + numberOfBits));
    extractor >>= (bitsOnStreamBlock - numberOfBits);


    readInt = extractor;
    this->readByteStreamBlockOffset += numberOfBits;
    if (this->readByteStreamBlockOffset == 64) {
        this->readByteStreamBlockOffset = 0;
        this->readByteStreamVectorOffset += 1;
    }
}

void BitStream::BitReader(int &readInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableFromCurrentStream = bitsOnStreamBlock - this->readByteStreamBlockOffset;

    if (bitsAvailableFromCurrentStream >= numberOfBits) {
        return this->bitReader(readInt, numberOfBits);
    }

    int numberOfBitsFirstRead = bitsAvailableFromCurrentStream;
    int numberOfBitsSecondRead = numberOfBits - numberOfBitsFirstRead;

    int firstReadInt = 0;
    this->bitReader(firstReadInt, numberOfBitsFirstRead);
    this->bitReader(readInt, numberOfBitsSecondRead);

    readInt <<= numberOfBitsFirstRead;
    readInt |= firstReadInt;
    return;
}

void BitStream::BitReader64(uint64_t &readInt, int numberOfBits) {

    int bitsOnStreamBlock = sizeof(uint_fast64_t) * 8;
    int bitsAvailableFromCurrentStream = bitsOnStreamBlock - this->readByteStreamBlockOffset;

    if (bitsAvailableFromCurrentStream >= numberOfBits) {
        return this->bitReader64(readInt, numberOfBits);
    }

    int numberOfBitsFirstRead = bitsAvailableFromCurrentStream;
    int numberOfBitsSecondRead = numberOfBits - numberOfBitsFirstRead;

    uint64_t firstReadInt = 0;
    this->bitReader64(firstReadInt, numberOfBitsFirstRead);
    this->bitReader64(readInt, numberOfBitsSecondRead);

    readInt <<= numberOfBitsFirstRead;
    readInt |= firstReadInt;
    return;
}

