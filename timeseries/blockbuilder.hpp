#ifndef BLOCK_BUILDER_HPP
#define BLOCK_BUILDER_HPP

#include <bitset>
#include "../stream/bitstream.hpp"

class BlockBuilder {

public:

    BlockBuilder();
    ~BlockBuilder();

    void WritePoint(int timestamp);
    std::vector<int> ReadBackPoints();
    std::vector<std::bitset<64>> ReadBackBitstream();
    
private:

    void Serialize();

    std::unique_ptr<BitStream> bitStream{
        std::make_unique<BitStream>()
    };

    int lastDelta{0};
    int bitOffset{0};
    uint64_t blockStart{0};
    int previousTimestamp{-1};
    int pointsWritten{0};
};

#endif