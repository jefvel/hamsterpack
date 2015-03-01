#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

#include "tinydir.h"
#include "miniz.h"

using namespace std;

string relativeDir;
string rootDir;

bool readFileIntoVector(string path, vector<char>& result){
    ifstream file;
    file.open(path.c_str(), ios::binary);
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    if(size == -1){
        file.close();
        return false;
    }

    result = vector<char>(size);

    if(file.read(result.data(), size)){
        file.close();
        return true;
    }

    file.close();
    return false;
}

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

bool processDirectory(string input, string output){
    tinydir_dir dir;
    string zipfile = output;
    zipfile.append(".zip");

    if(tinydir_open(&dir, input.c_str()) == -1){
        cout<<"Could not open dir.\n";
        return false;
    }

    if(rootDir == ""){
        rootDir = dir.path;
    }

    vector<string> dirs; 
    while(dir.has_next){
        tinydir_file file;
        if(tinydir_readfile(&dir, &file) == -1){
            cout<<"Could not read file.\n";
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
        processDirectory(dirs[i], output);
    }
}

bool createArchive(string input, string output){
    remove(output.c_str());

    //Create sample file.
    ofstream outfile;
    outfile.open(output.c_str());

    vector<char> headercontent;
    readFileIntoVector("src/hamsterimpl.cc", headercontent);

    outfile<<headercontent.data();


    outfile.close();

    string zipfile = output;
    zipfile.append(".zip");
    remove(zipfile.c_str());

    rootDir = "";
    relativeDir = "";

    processDirectory(input, output);
}

int writtenbytes;
char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
void writeBuffer(const char* buf, const size_t length,  ofstream& out){
    for(unsigned int i = 0; i < length; i ++){
        char const b = buf[i];
        out<<"0x"
           <<hex_chars[ ( b & 0xF0 ) >> 4 ]
           <<hex_chars[ ( b & 0x0F ) >> 0 ]
           <<",";
        writtenbytes ++;
        if(writtenbytes % 15 == 0) {
            out<<"\n";
        }
    }
}

void writeHeader(string path){
    writtenbytes = 0;
    string zipfile = path;
    zipfile.append(".zip");

    ifstream infile(zipfile.c_str(), ios::binary);
    ofstream outfile;
    outfile.open(path.c_str(), ios::binary | ios::app);

    outfile<<"const char HamsterPack::hamster_data[] = {\n";
    size_t buf_size = 1024;

    if(infile.is_open() && outfile.is_open()){
        char *buffer = new char[buf_size];
        while(infile.readsome(buffer, buf_size)){
            writeBuffer(buffer, infile.gcount(), outfile);
        }
    }

    outfile<<"0x00 };\n"
           <<"const size_t HamsterPack::hamster_size = "<<writtenbytes<<";\n";
    infile.close();
}

void removeZipFile(const string& filepath){
    string zippath = filepath;
    zippath.append(".zip");
    remove(zippath.c_str());
}

int main(int argc, char** args){
    if(argc != 3){
        std::cout<<"usage: "
            <<args[0]
            <<" [input dir] [output file]\n";
        return 0;
    }

    string input_dir = args[1];
    string out_file = args[2];

    if(input_dir.length() == 0 || out_file.length() == 0){
        return 0;
    }

    createArchive(input_dir, out_file);
    writeHeader(out_file);
    removeZipFile(out_file);
}
