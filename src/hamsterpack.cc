#include "hamsterpack.h"
#include "miniz.h"

//////////////////
//Implementation//
//////////////////
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;
HamsterPack::HamsterPack(){
    memset(&mArchive, 0, sizeof(mArchive));
    mz_bool status;
    status = mz_zip_reader_init_mem(&mArchive, hamster_data, hamster_size, 0);

    mInited = (status != 0);
}

HamsterPack::~HamsterPack(){
    if(mInited){
        mz_zip_reader_end(&mArchive);
    }
}

bool HamsterPack::loadFile(const string& filename, vector<unsigned char>& data){
    if(!mInited){
        return false;
    }

    void* p;
    size_t size;
    p = mz_zip_reader_extract_file_to_heap(&mArchive, filename.c_str(), &size, 0);

    if(p != NULL){
        data = vector<unsigned char>(size);
        memcpy(data.data(), p, size);
        return true;
    }

    return false;
}

bool HamsterPack::loadString(const string& filename, string& result){
    vector<unsigned char> data;
    bool res = loadFile(filename, data);
    if(res){
        result = string((char*)data.data());
        return true;
    }
    return false;
}

