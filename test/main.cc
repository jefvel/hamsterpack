#include <iostream>
#include <vector>

#include "hamsterpack.h"

int main(int args, char** argc){
    std::string test;

    HamsterPack pack;

    pack.loadString("lorem.txt", test);
    std::cout<<test<<" <- test.txt\n";

    pack.loadString("dir1/test1.txt", test);
    std::cout<<"HE:"<<test;

    int hej = 0;
    return hej;
}
