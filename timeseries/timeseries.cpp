
#include "../timeseries/timeseries.hpp"

TimeSeries::TimeSeries(int64_t metricID) {

    this->metricID = metricID;
    this->openBlock = std::make_unique<BlockBuilder>(this->metricID);

}

TimeSeries::~TimeSeries() {

    if (this->closedBlocks != nullptr) {
        std::free(this->closedBlocks);
    }

}

std::vector<std::pair<int, double>> 
    TimeSeries::Query(int startingTimestamp, int endpointTimestamp) {

    // rl broken due to how downstream offsets are pcd 
    this->writeLock.lock();

    auto data = std::vector<std::pair<int, double>>{};

    // need to store cached checkpoints
    this->openBlock->ResetReadOffsets();

    auto lazyQuery = this->openBlock->ReadOutData();
    for (auto &pair: lazyQuery) {
        // pair < timestamp, value > 
        if ( startingTimestamp >= std::get<0>(pair) && std::get<0>(pair) <= endpointTimestamp ) {
            data.emplace_back(std::get<0>(pair), std::get<1>(pair));
        }

        if (std::get<0>(pair) > endpointTimestamp) {
            this->writeLock.unlock();
            return data;
        }
    }

    // copy starting pointer and start reading out closed blocks....
    char *closedBlockRef = this->closedBlocks;
    for (int i = 0; i < storedBlocks; i++) {

        auto lazyQuery = BlockBuilder::Deserialize(closedBlockRef)->ReadOutData();

        for (auto &pair: lazyQuery) {
            // pair < timestamp, value > 
            if ( startingTimestamp >= std::get<0>(pair) && std::get<0>(pair) <= endpointTimestamp ) {
                data.emplace_back(std::get<0>(pair), std::get<1>(pair));
            }

            if (std::get<0>(pair) > endpointTimestamp) {
                this->writeLock.unlock();
                return data;
            }
        }

    }


    this->writeLock.unlock();
    return data;

}

void TimeSeries::WriteSeries(int timestamp, double dataValue) {

    this->writeLock.lock();

    if ( !this->openBlock->WriteSeries(timestamp, dataValue) ) {
        if (this->closedBlocks == nullptr) {
            this->blockSizeBytes += this->openBlock->SizeInBytes();
            this->closedBlocks = (char *) std::malloc(this->openBlock->SizeInBytes());

            char *closedBlocksCopy = this->closedBlocks;
            this->openBlock->Serialize(closedBlocksCopy);
        } else {
            this->closedBlocks = (char *) std::realloc(this->closedBlocks, this->blockSizeBytes + this->openBlock->SizeInBytes());
            
            char *closedBlocksWriteOffset = this->closedBlocks + this->blockSizeBytes;
            this->blockSizeBytes += this->openBlock->SizeInBytes();
            this->openBlock->Serialize(closedBlocksWriteOffset);
        }

        this->storedBlocks += 1;
        this->openBlock = std::make_unique<BlockBuilder>(this->metricID);

    }
    
    this->writeLock.unlock();

}