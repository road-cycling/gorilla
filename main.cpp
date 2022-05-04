#include "stream/bitstream.hpp"
#include "timeseries/blockbuilder.hpp"

#include <bitset>
#include <iostream>
#include <vector>

int main() {
    
    auto foo = new BlockBuilder();

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

    for (auto &ap : writePoints) {
        foo->WritePoint(ap);
    }


    for (auto &ap : writePoints) {
        std::cout << foo->ReadPoint() << std::endl;
    }

    // auto foo = new BlockBuilder();
    // foo->WriteDouble(12);
    // foo->WriteDouble(24);
    // foo->WriteDouble(15);
    // foo->WriteDouble(12);
    // foo->WriteDouble(35);

    // foo->WriteDouble(15.5);
    // foo->WriteDouble(14.0625);
    // foo->WriteDouble(3.25);
    // foo->WriteDouble(8.625);
    // foo->WriteDouble(13.1);
    // foo->WriteDouble(987654321.12345);



    // auto readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // readPoint = foo->ReadDouble();
    // std::cout << readPoint << std::endl;

    // double value_float = 15.5;


    // uint64_t value_uint = 0;

    // std::memcpy(&value_uint, &value_float, 8);

    // std::cout << value_uint << std::endl;
    // double value_float = 1.;
    // uint64_t value_uint = 0;


    // std::memcpy(&value_uint, &value_float, 8);
    // std::cout << value_uint << std::endl;

    // std::bitset<64>doubleBitset(value_uint);
    // std::cout << "DoubleBitset for " << value_float << " is: ";
    // std::cout << doubleBitset << std::endl;

    // std::cout << std::hex << value_uint << std::endl;


    // std::vector<double> testVector{12,24,15,12,35};

    // auto foo = new BlockBuilder();
    // for (auto a: testVector) {
    //     foo->WriteDouble(a);
    // }

    // uint64_t xorWithPrevious = 0;

    // for (auto item: testVector ){
    //     std::memcpy(&value_uint, &item, 8);
    //     // std::cout << std::hex << value_uint << std::endl;

    //     xorWithPrevious ^= value_uint;
    //     std::cout << std::hex << xorWithPrevious << std::endl;

    // }

    // for (int i = 0; i < testVector.size(); i++) {
    //     value_uint = 0;
    //     std::memcpy(&value_uint, &testVector.at(i), 8);
    //     xorWithPrevious ^= value_uint;
    //     std::cout << std::hex << xorWithPrevious << std::endl;
    // }


    // double _24 = 12;
    // double _15 = 24;

    // uint64_t __24 = 0;
    // uint64_t __15 = 0;

    // std::memcpy(&__24, &_24, 8);
    // std::memcpy(&__15, &_15, 8);

    // uint64_t xorr = __24 ^ __15;
    // std::cout << std::hex << xorr << std::endl;

}

// int main() {

    // g++ -std=c++20 main.cpp stream/bitstream.cpp timeseries/blockbuilder.cpp -o prog -Dprod -DDEBUG && ./prog && rm prog

//     // g++ -std=c++20 main.cpp stream/bitstream.cpp -o prog -Dprod && ./prog && rm prog
//     // g++ -std=c++20 main.cpp stream/bitstream.cpp timeseries/blockbuilder.cpp -o prog -Dprod -Ddebug && ./prog && rm prog

//     /*
//     std::cout << "Hello World" << std::endl;

//     float foobar = 1444;
//     std::cout << foobar << std::endl;
//     std::cout << sizeof(foobar) << std::endl;
//     std::bitset<64>bs(foobar);
//     std::cout << bs << std::endl;

//     uint64_t test1 = 1;


//     std::cout << sizeof(foobar) << std::endl;
//     std::cout << sizeof(test1) << std::endl;

//     std::cout << std::hex << foobar << std::endl;
//     */

//     // while(test1) {
//     //     if (test1 & foobar) {
//     //         std::cout << "1";
//     //     } else {
//     //         std::cout << "0";
//     //     }
//     //     test1 <<=1;
//     // }
//     // std::cout << std::endl;


//     // float f = 1.5;
//     // uint64_t f_as_int = 0;
//     // std::memcpy(&f_as_int, &f, sizeof(float));
//     // std::bitset<64>f_as_bitset{f_as_int};
//     // std::cout << f_as_bitset << std::endl;

//     // // float64

//     // double d = 15.5;
//     // std::cout << std::hex << d << std::endl;

//     // int x = 255;
//     // std::cout << std::hex << x << std::endl;

//     // double value = 1;
//     // uint64_t *p = (uint64_t *)&value;

//     // uint64_t p_copy = *p;

//     // std::cout << p_copy << std::endl;

//     // std::bitset<64>doubleBitset(p_copy);
//     // std::cout << doubleBitset << std::endl;

//     // double value_float = 2;
//     // // uint64_t value_64 = (uint64_t) value;
//     // // std::cout << value_64 << std::endl;

//     // uint64_t value_uint = 0;

//     // std::cout << sizeof(value_float) << std::endl;
//     // std::cout << sizeof(value_uint) << std::endl;

//     // std::memcpy(&value_uint, &value_float, 8);
//     // std::cout << value_uint << std::endl;

//     // std::bitset<64>doubleBitset(value_uint);
//     // std::cout << doubleBitset << std::endl;

//     // std::cout << std::hex << value_uint << std::endl;

//     // // // double value_float = 0b0/**/00000000001/**/0000000000000000000000000000000000000000000000000000;
//     // double value_floatt = 0b0000000000010000000000000000000000000000000000000000000000000000;
//     // std::cout << value_floatt << std::endl;


//     // double copyback = 0;
//     // std::memcpy(&copyback, &value_float, 8);
//     // std::cout << copyback << std::endl;

//     double value_float = 1.5;
//     uint64_t value_uint = 0;


//     std::cout << sizeof(value_float) << std::endl;

//     std::memcpy(&value_uint, &value_float, 8);
//     std::cout << value_uint << std::endl;

//     std::bitset<64>doubleBitset(value_uint);
//     std::cout << doubleBitset << std::endl;

//     // std::cout << std::hex << value_uint << std::endl;

//     // // double value_float = 0b0/**/00000000001/**/0000000000000000000000000000000000000000000000000000;
//     // double value_floatt = 0b0000000000010000000000000000000000000000000000000000000000000000;
//     // std::cout << value_floatt << std::endl;
// }



// 0000000000000000000000000000000000111111110011110001101010100000