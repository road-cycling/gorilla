#include "../stream/bitstream.hpp"
#include <iostream>


void VerifyWriteReadBackBasicOps_1();
void VerifyWriteReadBackBasicOps_2();
void VerifyWriteReadBackBasicOps_3();

void VerifyWriteReadBoundryOps_1();

void ASSERT_EQ(int a, int b) {
    if (a != b) {
        exit(1);
    }
}

int main() {
    VerifyWriteReadBackBasicOps_1();
    VerifyWriteReadBackBasicOps_2();
    VerifyWriteReadBackBasicOps_3();
    VerifyWriteReadBoundryOps_1();
}

void VerifyWriteReadBackBasicOps_1() {
    auto bitstream = std::make_unique<BitStream>();

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


void VerifyWriteReadBackBasicOps_2() {
    auto bitstream = std::make_unique<BitStream>();

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

void VerifyWriteReadBackBasicOps_3() {

    auto bitstream = std::make_unique<BitStream>();

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

void VerifyWriteReadBoundryOps_1() {
    
    auto bitstream = std::make_unique<BitStream>();
    
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


