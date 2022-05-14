#ifndef BLOCK_BUILDER_HPP
#define BLOCK_BUILDER_HPP

#include <bitset>
#include "bitstream.hpp"
#include <istream>
#include <iostream>

class BlockBuilder {

public:

    BlockBuilder();

    BlockBuilder(int64_t metricId):
        _metric_id(metricId) {};

    ~BlockBuilder();

    bool WriteSeries(int timestamp, double dataValue);
    bool WritePoint(int timestamp);
    void WriteDouble(double dataValue);

    double ReadDouble();
    int ReadPoint();

    std::vector<std::pair<int, double>> ReadOutData();

    std::vector<int> ReadBackPoints();
    std::vector<std::bitset<64>> ReadBackBitstream();

    int bitsAllocated() {
        return this->bitStream->byteStream->size() * 64;
    }

    void ResetReadOffsets();

    void Serialize(std::ostream &writer);
    static std::unique_ptr<BlockBuilder> Deserialize(std::istream &writer);

    void Serialize(char *&writer);
    static std::unique_ptr<BlockBuilder> Deserialize(char *&reader);

    int SizeInBytes() {
        return sizeof(this->_metric_id) + \
            sizeof(this->blockStart) + \
            sizeof(this->pointsWritten) + \
            this->bitStream->SizeInBytes();

    }

public:


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

    /* metric UID */
    int64_t _metric_id;
};

#endif