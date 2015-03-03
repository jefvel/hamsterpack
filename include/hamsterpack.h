#pragma once
#include "tinydir.h"
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.h"
#undef MINIZ_HEADER_FILE_ONLY
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class HamsterPack {
private:
    static const unsigned char hamster_data[];
    static const size_t hamster_size;

     mz_zip_archive mArchive;
     bool mInited;
public:
     HamsterPack();
    ~HamsterPack();
    bool loadFile(const string& filename, vector<char>& data);
    bool loadString(const string& filename, string& result);
};
