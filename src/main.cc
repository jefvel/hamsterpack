#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

#include "tinydir.h"
#include "miniz.h"

using namespace std;
namespace HamPack {
bool readFileIntoVector(string path, vector<char>& result){
    ifstream file;
    file.open(path.c_str(), ios::binary);
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    if(size <= 0){
        file.close();
        return false;
    }

    result = vector<char>((unsigned int)size);

    if(file.read(result.data(), size)){
        file.close();
        return true;
    }

    file.close();
    return false;
}

string relativeDir;
string rootDir;

///Returns the path name for the current zip dir we are in.
string getZipPath(){
    string result;

    result = relativeDir.substr(rootDir.length() + 1);

    int pos = result.find_last_of('/');

    if(pos == -1){
        pos = 0;
    }

    result = result.substr(0, pos);

    if(result.length() > 0){
        result.append("/");
    }

    return result;
}

mz_bool addDir(string zipfile, tinydir_file &dir){
    string name = getZipPath();
    name.append(dir.name);
    name.append("/");

    return mz_zip_add_mem_to_archive_file_in_place(
            zipfile.c_str(),
            name.c_str(), NULL, 0, NULL, 0, MZ_BEST_COMPRESSION);
}

mz_bool addFile(string zipfile, tinydir_file& infile){
    string name = getZipPath();
    name.append(infile.name);
    vector<char> buffer;

    if(!readFileIntoVector(infile.path, buffer)){
        return false;
    }

    return mz_zip_add_mem_to_archive_file_in_place(
            zipfile.c_str(),
            name.c_str(),
            buffer.data(), buffer.size(), NULL, 0, MZ_BEST_COMPRESSION);
}

bool processDirectory(string input, string zipfile){
    tinydir_dir dir;

    if(tinydir_open(&dir, input.c_str()) == -1){
        cout<<"Could not open dir."<<endl;
        return false;
    }

    if(rootDir == ""){
        rootDir = dir.path;
    }

    vector<string> dirs; 
    while(dir.has_next){
        tinydir_file file;
        if(tinydir_readfile(&dir, &file) == -1){
            cout<<"Could not read file."<<endl;
            continue;
        }

        relativeDir = file.path;

        if(file.is_dir){
            string name = file.name;
            //Don't add the dirs . and ..
            if(name != "." && name != ".."){
                addDir(zipfile, file);
                dirs.push_back(file.path);
            }
        }else{
            addFile(zipfile, file);
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);

    for(unsigned int i = 0; i < dirs.size(); i++){
        processDirectory(dirs[i], zipfile);
    }

    return true;
}

bool createArchive(string input, string output){
    string zipfile = output;
    zipfile.append(".new.zip");
    remove(zipfile.c_str());

    rootDir = "";
    relativeDir = "";

    processDirectory(input, zipfile);
	
	return true;
}

int writtenbytes;
char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
inline void writeBuffer(const char* buf, const size_t length,  ofstream& out){
    for(unsigned int i = 0; i < length; i ++){
        char const b = buf[i];
        out<<"0x"
           <<hex_chars[ ( b & 0xF0 ) >> 4 ]
           <<hex_chars[ ( b & 0x0F ) >> 0 ]
           <<",";
        writtenbytes ++;
        if(writtenbytes % 15 == 0) {
            out<<endl;
        }
    }
}

bool zipDiffers(string path){
    string oldPath = path+".zip";
    string newPath = path+".new.zip";
    ifstream oldFile(oldPath.c_str(), ios::binary | ios::in);

    streamsize oldSize = 0;
    streamsize newSize = 0;
    
    if(oldFile.is_open()){
        oldFile.seekg(0, ios::end);
        oldSize = oldFile.tellg();
        oldFile.seekg(0, ios::beg);
    }

    if(oldSize == 0){
        return true;
    }

    ifstream newFile(newPath.c_str(), ios::binary | ios::in);
    if(newFile.is_open()){
        newFile.seekg(0, ios::end);
        newSize = newFile.tellg();
        newFile.seekg(0, ios::beg);
    }

    return oldSize != newSize;

    if(oldSize != newSize){
        return true;
    }
    
    const size_t buf_size = 2048;
    char buf1[buf_size];
    char buf2[buf_size];


    bool differs = false;
    while(newFile.good() && oldFile.good()){
        oldFile.read(buf1, buf_size);
        newFile.read(buf2, buf_size);

        if(memcmp(buf1, buf2, buf_size) != 0){
            differs = true;
            break;
        }

        memset(buf1, 0, buf_size);
        memset(buf2, 0, buf_size);

    }

    oldFile.close();
    newFile.close();

    return differs;

}

void replaceOldZip(string path){
    string oldPath = path+".zip";
    string newPath = path+".new.zip";
    ofstream oldZip;
    ifstream newZip;

    oldZip.open(oldPath.c_str(), ios::binary);
    newZip.open(newPath.c_str(), ios::binary);
    const size_t buf_size = 1024 << 1;
    char buf[buf_size];
    while(newZip.good()){
        newZip.read(buf, buf_size);
        streamsize s = newZip.gcount();
        oldZip.write(buf, s);
        if(s == 0){
            break;
        }
    }

    oldZip.close();
    newZip.close();

    remove(newPath.c_str());
}

void writeHamsterFile(string path){
    if(!zipDiffers(path)){
        cout<<"Data file up to date, no need to rewrite it."<<endl;
        return;
    }

    cout<<"Data modified, creating new header"<<endl;

    remove(path.c_str());

    ofstream outfile;
    outfile.open(path.c_str(), ios::binary);
    outfile<<"#include \"hamsterpack.h\""<<endl;

    writtenbytes = 0;
    string zipfile = path;
    zipfile.append(".new.zip");
    ifstream infile(zipfile.c_str(), ios::binary);

    outfile<<"const unsigned char HamsterPack::hamster_data[] = {"<<endl;
    const size_t buf_size = 1024 << 5;

    if(infile.is_open() && outfile.is_open()){
        char buffer[buf_size];
        while(infile.good()){
            infile.read(buffer, buf_size);
            writeBuffer(buffer, (size_t)infile.gcount(), outfile);
        }
    }

    outfile<<"0x00 };"<<endl
           <<"const size_t HamsterPack::hamster_size = "<<writtenbytes<<";"<<endl;
    infile.close();
}
};

int main(int argc, char** args){
    if(argc != 3){
        std::cout<<"usage: "
            <<args[0]
            <<" [input dir] [output file]"<<endl;
        return 0;
    }

    string input_dir = args[1];
    string out_file = args[2];

	//cout<<"Processing dir "<<input_dir<<" into file "<<out_file<<endl;

    if(input_dir.length() == 0 || out_file.length() == 0){
        return 0;
    }

    HamPack::createArchive(input_dir, out_file);
    HamPack::writeHamsterFile(out_file);
    HamPack::replaceOldZip(out_file);
}
