#include "bitstream.hpp"

#include <iostream>


BitStream::BitStream(){
    this->byteStream->push_back(0);
}

BitStream::~BitStream(){}

void BitStream::bitWriter(uint_fast64_t writeInt, int numberOfBits) {
    this->byteStream->back() |= (writeInt << this->writeByteStreamStartingOffset);
    this->writeByteStreamStartingOffset += numberOfBits;
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
        std::cout << "Hit offset" << std::endl;
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


/*
TESTS

 - cmake -S . -B build && cmake --build build
 - cd build && ctest
*/

#ifndef prod

#include <gtest/gtest.h>

TEST(BS, VerifyWriteReadBackBasicOps_1) {

    auto bitstream = std::make_unique<BitStream>();
    ASSERT_NE(bitstream, nullptr);

    for (int i = 0; i < 50000; i++) {
        bitstream->WriteBits(1, 1);
    }
    bitstream->WriteBits(0, 1);

    for (int i = 0; i < 50000; i++) {
        int readerInt = 0;
        bitstream->BitReader(readerInt, 1);
        ASSERT_EQ(readerInt, 1);
    }

    int readerInt = 0;
    bitstream->BitReader(readerInt, 1);
    ASSERT_EQ(readerInt, 0);

}

TEST(BS, VerifyWriteReadBackBasicOps_2) {

    auto bitstream = std::make_unique<BitStream>();
    ASSERT_NE(bitstream, nullptr);

    for (int i = 0; i < 5000000; i++) {
        bitstream->WriteBits(16, 16);
    }
    bitstream->WriteBits(0, 1);

    for (int i = 0; i < 5000000; i++) {
        int readerInt = 0;
        bitstream->BitReader(readerInt, 16);
        ASSERT_EQ(readerInt, 16);
    }

    int readerInt = 0;
    bitstream->BitReader(readerInt, 1);
    ASSERT_EQ(readerInt, 0);

}

TEST(BS, VerifyWriteReadBackBasicOps_3) {

    auto bitstream = std::make_unique<BitStream>();
    ASSERT_NE(bitstream, nullptr);

    for (int i = 0; i < 5000000; i++) {
        bitstream->WriteBits(16, 15);
    }
    bitstream->WriteBits(0, 1);

    for (int i = 0; i < 5000000; i++) {
        int readerInt = 0;
        bitstream->BitReader(readerInt, 15);
        ASSERT_EQ(readerInt, 16);
    }

    int readerInt = 0;
    bitstream->BitReader(readerInt, 1);
    ASSERT_EQ(readerInt, 0);

}

TEST(BS, VerifyWriteReadBoundryOps_1) {

    auto bitstream = std::make_unique<BitStream>();
    ASSERT_NE(bitstream, nullptr);

    
    bitstream->WriteBits(1, 16);
    bitstream->WriteBits(1, 16);
    bitstream->WriteBits(1, 16);
    bitstream->WriteBits(1, 15);
    bitstream->WriteBits(15, 4);

    std::vector<std::pair<int, int>> bQExpected = {
        {16, 1},
        {16, 1},
        {16, 1},
        {15, 1},
        {4, 15}
    };

    for (auto expected : bQExpected) {
        int readerInt = 0;
        bitstream->BitReader(readerInt, std::get<0>(expected));
        ASSERT_EQ(readerInt, std::get<1>(expected));
    }


}


#endif