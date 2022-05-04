#ifndef BLOCK_BUILDER_HPP
#define BLOCK_BUILDER_HPP

#include <bitset>
#include "../stream/bitstream.hpp"

class BlockBuilder {

public:

    BlockBuilder();
    ~BlockBuilder();

    void WritePoint(int timestamp);
    void WriteDouble(double dataValue);

    double ReadDouble();
    int ReadPoint();

    std::vector<std::pair<int, double>> ReadOutData();

    std::vector<int> ReadBackPoints();
    std::vector<std::bitset<64>> ReadBackBitstream();
    
private:

    void Serialize();

    std::unique_ptr<BitStream> bitStream{
        std::make_unique<BitStream>()
    };

    /* Writing Timestamps Tracking Start */
    int lastDelta{0};
    int bitOffset{0};
    uint64_t blockStart{0};
    int previousTimestamp{-1};
    int pointsWritten{0};
    /* Writing Timestamps Tracking End */

    /* Reading Timestamps Start */
    bool timestampHasBeenReadFirstTime{false};
    int lastReadTimestamp{0};
    int trailingDelta{0};
    /* Reading Timestamps End */

    /* Writing Values (Doubles) Start */
    bool doubleHasBeenWritten{false};
    uint64_t previousDoubleWritten{0};

    int previouslyWrittenTrailingZeroes{-1};
    int previouslyWrittenLeadingZeroes{-1};
    /* Writing Values (Doubles) End*/

    /* Reading Values (Doubles) Start */
    bool firstDoubleRead{false};
    uint64_t lastDoubleRead{0};
    int lastReadTrailingZeroes{-1};
    int lastReadLeadingZeroes{-1};
    /* Reading Values (Doubles) End */
};

#endif