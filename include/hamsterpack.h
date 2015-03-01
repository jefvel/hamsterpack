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
    static const char hamster_data[];
    static const size_t hamster_size;

    static mz_zip_archive zip_archive;
    static bool sInited;
public:
    static bool init();
    static void exit();
    static bool readFile(const string& filename, vector<char>& data);
    static bool loadString(const string& filename, string& result);
};
