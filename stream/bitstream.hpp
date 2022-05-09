#ifndef BITSTREAM_HPP
#define BITSTREAM_HPP

#include <vector>

class BitStream {
    public:
        BitStream();
        ~BitStream();

        void print();
        
        void WriteBits(int writeInt, int numberOfBits);
        void WriteBits(uint64_t writeInt, int numberOfBits);

        void BitReader(int &readInt, int numberOfBits);
        void BitReader64(uint64_t &readInt, int numberOfBits);


        static void WriteToFlipPotentialNegative(int &inputInt, int numberOfBits);

        void Serialize(std::ostream &writer);
        void Deserialize(std::istream &reader);

        void Serialize(char *&writer);
        void Deserialize(char *&reader);

        int SizeInBytes() {
            return this->byteStream->size() * sizeof(uint_fast64_t);
        }

    private:
        void bitReader64(uint64_t &readInt, int numberOfBits);
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