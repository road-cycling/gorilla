#ifndef BITSTREAM_HPP
#define BITSTREAM_HPP

#include <vector>

class BitStream {
    public:
        BitStream();
        ~BitStream();

        void WriteBits(int writeInt, int numberOfBits);

        void BitReader(int &readInt, int numberOfBits);

    private:
        void bitReader(int &readInt, int numberOfBits);
        void bitWriter(uint_fast64_t writeInt, int numberOfBits);

    public:
        int writeByteStreamStartingOffset{0};
        std::unique_ptr<std::vector<uint_fast64_t>> byteStream{
            std::make_unique<std::vector<uint_fast64_t>>()
        };

        int readByteStreamBlockOffset{0};
        int readByteStreamVectorOffset{0};
};

#endif