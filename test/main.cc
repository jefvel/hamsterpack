#include <iostream>
#include <vector>

#include "hamsterpack.h"

int main(int args, char** argc){
    std::string test;

    HamsterPack pack;

    pack.loadString("lorem.txt", test);
    std::cout<<test<<" <- test.txt\n";

    int hej = 0;
    return hej;
}
