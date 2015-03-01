# HamsterPack
Packs zipped directory into executable, which then can be read out of.

#Usage
To use hamsterpack, modify your CMakeLists with this:        

Add the source file
    hamsterfile.cc                                                                     
to your executable.

Include_directories to hamsterpacks include dir:
    include_directories(hamsterpack/include)                        

Set HAM_IN_DIR to the data dir you want to embed (absolute path):
    set(HAM_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

Add hamsterpack as a subdir:
    subdirs(hamsterpack)

After that you can include hamsterpack.h in your program, after which you have access to these functions:
    //Has to be run first. Returns true if stuff works.
    HamsterPack::init();
    //Reads file into result, returns true if succeeded.
    HamsterPack::readFile(string filename, vector<char>& result);
    //Parses file as text and puts it into result. Returns true when it succeeds.
    HamsterPack::loadString(string filename, string& result);
    //Closes archive.
    HamsterPack::exit();

That's all!
Good bye.
