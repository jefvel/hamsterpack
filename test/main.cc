#include <iostream>
#include <vector>

#include "hamsterpack.h"

int main(int args, char** argc){
    std::string test;

    HamsterPack pack;

    pack.loadString("lorem.txt", test);
    std::cout<<test<<" <- test.txt\n";
    vector<unsigned char> d;
    pack.loadFile("dir1/test1.txt", d);
    std::cout<<d.size()<<" size\n";
    pack.loadString("dir1/test1.txt", test);
    std::cout<<"HE:"<<test;

    int hej = 0;
    return hej;
}
