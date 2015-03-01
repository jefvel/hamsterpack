set(HAMSTERPACK_FOUND false)
find_path(
    HAMSTERPACK_INCLUDE_DIR
    include/hamsterpack.h
    PATH_SUFFIXES
    hamsterpack
    include
    PATHS
    ${HAMSTERPACK_ROOT}
    ${HAMSTERPACKDIR}
    $ENV{HAMSTERPACKDIR}
    $ENV{HAMSTERPACK_ROOT}
    /usr
    /usr/local
    ~/git/
    ~/Library/Frameworks
    /Library/Frameworks
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/hamsterpack"
    )

message(bajs ${HAMSTERPACK_INCLUDE_DIR})
