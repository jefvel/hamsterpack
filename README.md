# HamsterPack
Packs zipped directory into executable, which then can be read out of.

#Usage
To use hamsterpack, modify your CMakeLists with this:

    #Set data dir to embed (defaults to data, so setting it is optional)
    set(HAM_IN_DIR data)
    #change path_to_hamsterpack to where you put it
    include([path_to_hamsterpack]/HamsterInclude.txt)

After that you can include hamsterpack.h in your program, then you can do this:

    HamsterPack pack;
    vector<char> data;
    bool loaded = pack.loadFile("test.data", data);

    string testString;
    loaded = pack.loadString("textfiles/readme.txt", testString);
    cout<<testString;

That's all!
Good bye.
