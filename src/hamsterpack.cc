#include "hamsterpack.h"
#include "miniz.h"

//////////////////
//Implementation//
//////////////////
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

bool HamsterPack::loadFile(const string& filename, vector<char>& data){
    if(!mInited){
        return false;
    }

    void* p;
    size_t size;
    p = mz_zip_reader_extract_file_to_heap(&mArchive, filename.c_str(), &size, 0);

    if(p != NULL){
        data = vector<char>(size);
        memcpy(data.data(), p, size);
        return true;
    }

    return false;
}

bool HamsterPack::loadString(const string& filename, string& result){
    vector<char> data;
    bool res = loadFile(filename, data);
    if(res){
        result = string(data.data());
        return true;
    }
    return false;
}

