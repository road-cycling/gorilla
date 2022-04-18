// #include "stream/bitstream.hpp"
#include "timeseries/blockbuilder.hpp"

#include <iostream>

int main() {

    // g++ -std=c++14 main.cpp stream/bitstream.cpp -o prog -Dprod && ./prog && rm prog
    // g++ -std=c++14 main.cpp stream/bitstream.cpp timeseries/blockbuilder.cpp -o prog -Dprod && ./prog && rm prog

    // auto bs = new BitStream();

    auto bb = new BlockBuilder();

    std::cout << "Hello World" << std::endl;
    std::cout << bb << std::endl;


    bb->WritePoint(1647835200);
    bb->WritePoint(1647835260);
    bb->WritePoint(1647835320);
    bb->WritePoint(1647835380);
    bb->WritePoint(1647835440);
    bb->WritePoint(1647835500);
    bb->WritePoint(1647835560);
    bb->WritePoint(1647835620);
    bb->WritePoint(1647835680);
    bb->WritePoint(1647835745);
    bb->WritePoint(1647835800);
    bb->WritePoint(1647835859);

    // bb->WritePoint(1647835201);
    // bb->WritePoint(1647835260);
    // bb->WritePoint(1647835266);
    // bb->WritePoint(1647835285);



    // bb->WritePoint(1647838860);
    // bb->WritePoint(1647838920);
    // bb->WritePoint(1647838981);
    // bb->WritePoint(1647839043);
    // bb->WritePoint(1647839100);
    // bb->WritePoint(1647839160);
    // bb->WritePoint(1647839220);
    // bb->WritePoint(1647839280);
    // bb->WritePoint(1647839340);
    // bb->WritePoint(1647839400);

    // 2 hours = 60 * 60 * 2 = 7200

    

    auto pts = bb->ReadBackPoints();
    std::cout << "read back" << std::endl;
    for (auto a: pts) {
        std::cout << a << std::endl;
    }

    // int first = 0b00000000000000000000111001001100;
    // int second = 0b11111111111100011111000011110000;
    
    // std::cout << first << std::endl;
    // std::cout << second << std::endl;

    // int foo = -1;
    // std::bitset<64> b(foo);
    // std::cout << b << std::endl;

    // int boo = -10;
    // std::bitset<64> c(boo);
    // std::cout << c << std::endl;

    // boo *= -1;
    // std::bitset<64> d(boo);
    // std::cout << d << std::endl;


}