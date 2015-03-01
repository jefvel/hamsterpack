#include <iostream>
#include <vector>

#include "hamsterpack.h"

int main(int args, char** argc){
    HamsterPack::init();

    std::string test;
    HamsterPack::loadString("lorem.txt", test);
    std::cout<<test<<" <- test.txt\n";
    HamsterPack::exit();

    int hej = 0;
    return hej;
}
