#include "blockbuilder.hpp"

#include <iostream>

#define _0    0
#define _01   1   // 10
#define _011  3   // 110
#define _0111 7   // 1110
#define _1111 15  // 1111

BlockBuilder::BlockBuilder(){}
BlockBuilder::~BlockBuilder(){}
void BlockBuilder::Serialize(){}

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

std::vector<int> BlockBuilder::ReadBackPoints() {

    std::vector<int> decompressedTimeSeries;

    int lastTimestamp = this->blockStart;
    int trailingDelta = 0;

    int delta = 0;
    int firstBitFromStream = 0;
    for (int i = 0; i < this->pointsWritten; i++) {

        delta = 0;
        firstBitFromStream = 0;
        this->bitStream->BitReader(firstBitFromStream, 1);

        // 0
        if (firstBitFromStream == 0) {
            #ifdef DEBUG
                std::cout << "1Δ=0" << std::endl;
            #endif

            lastTimestamp = lastTimestamp + trailingDelta;
            decompressedTimeSeries.push_back(lastTimestamp);

            continue;
        }

        firstBitFromStream = 0;
        this->bitStream->BitReader(firstBitFromStream, 1);

        // 1 0 = cap = 64
        if (firstBitFromStream == 0) {
            this->bitStream->BitReader(delta, 7);

            if (delta > 64) {
                delta ^= (1 << 6);
                delta *= -1;
            }

            #ifdef DEBUG
                std::cout << "2Δ=" << delta << std::endl;
            #endif

            lastTimestamp += (delta + trailingDelta);
            trailingDelta += delta;
            decompressedTimeSeries.push_back(lastTimestamp);

            continue;
        }

        firstBitFromStream = 0;
        this->bitStream->BitReader(firstBitFromStream, 1);

        // 1 1 0 = cap = 256
        if (firstBitFromStream == 0) {
            this->bitStream->BitReader(delta, 9);

            if (delta > 256) {
                delta ^= (1 << 8);
                delta *= -1;
            }

            #ifdef DEBUG
                std::cout << "3Δ=" << delta << std::endl;
            #endif

            lastTimestamp += (delta + trailingDelta);
            trailingDelta += delta;
            decompressedTimeSeries.push_back(lastTimestamp);

            continue;
        }

        firstBitFromStream = 0;
        this->bitStream->BitReader(firstBitFromStream, 1);

        // 1 1 1 0 = cap = 2048
        if (firstBitFromStream == 0) {
            this->bitStream->BitReader(delta, 12);

            if (delta > 2048) {
                delta ^= (1 << 11);
                delta *= -1;
            }

            #ifdef DEBUG
                std::cout << "4Δ=" << delta << std::endl;
            #endif

            lastTimestamp += (delta + trailingDelta);
            trailingDelta += delta;
            decompressedTimeSeries.push_back(lastTimestamp);

            continue;
        }

        this->bitStream->BitReader(delta, 32);

        #ifdef DEBUG
            std::cout << "4Δ=" << delta << std::endl;   
        #endif

        lastTimestamp += (delta + trailingDelta);
        trailingDelta += delta;
        decompressedTimeSeries.push_back(lastTimestamp);

    }

    return decompressedTimeSeries;

}

void BitStream::WriteToFlipPotentialNegative(int &inputInt, int numberOfBits) {
    if (0 > inputInt) {
        inputInt *= -1;
        inputInt |= (1<<(numberOfBits - 1));
    }
}