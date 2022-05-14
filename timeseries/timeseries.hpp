#ifndef TIMESERIES_HPP
#define TIMESERIES_HPP

#include "../stream/blockbuilder.hpp"

#include <memory>
#include <vector>
#include <shared_mutex>

class TimeSeries {
    public:

        TimeSeries(int64_t metricID);
        ~TimeSeries();

        void WriteSeries(int timestamp, double dataValue);
        std::vector<std::pair<int, double>> Query(int startingTimestamp, int endpointTimestamp);


    private:

    int64_t metricID;

    std::unique_ptr<BlockBuilder> openBlock;

    int storedBlocks{0};
    int blockSizeBytes{0};
    char* closedBlocks{nullptr};

    std::unique_lock<std::shared_mutex>writeLock;
    std::shared_lock<std::shared_mutex>readLock;
};

#endif