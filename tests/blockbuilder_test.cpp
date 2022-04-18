#include <iostream>
#include "../timeseries/blockbuilder.hpp"

void VerifyWriteOutAndReadBack();

void ASSERT_EQ(int a, int b) {
    if (a != b) {
        exit(1);
    }
}

void ASSERT_EQl(uint64_t a, uint64_t b) {
    if (a != b) {
        exit(1);
    }
}

int main() {
    VerifyWriteOutAndReadBack();
}

void VerifyWriteOutAndReadBack() {
    
    auto blockBuilder = std::make_unique<BlockBuilder>();
    auto writePoints = std::vector<int>{
        1647835200,
        1647835260,
        1647835320,
        1647835380,
        1647835440,
        1647835500,
        1647835560,
        1647835620,
        1647835680,
        1647835745,
        1647835800,
        1647835859
    };

    /*
    Δ: 0	 ΔΔ: 0
    Δ: 60	 ΔΔ: 60
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 60	 ΔΔ: 0
    Δ: 65	 ΔΔ: 5
    Δ: 55	 ΔΔ: -10
    Δ: 59	 ΔΔ: 4
    */

    for (auto wp: writePoints) {
        blockBuilder->WritePoint(wp);
    }

    auto readOutPoints = blockBuilder->ReadBackPoints();

    ASSERT_EQ(writePoints.size(), readOutPoints.size());

    for (int i = 0; i < writePoints.size(); i++) {
        ASSERT_EQ(writePoints.at(i), readOutPoints.at(i));
    }

    ASSERT_EQl(
        0b0000000000000000000000001000110010100100001010100000000111100010,
        blockBuilder->ReadBackBitstream().at(0).to_ulong()
    );

    std::cout << __func__ << " DONE" << std::endl;
}