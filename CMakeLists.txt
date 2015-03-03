set(parentproject ${PROJECT_NAME})

if(parentproject)
    set(HAM_OUT_FILE ${PROJECT_SOURCE_DIR}/hamstertrash/hamsterdata.cc)
    set(HAM_OUT_DIR ${PROJECT_SOURCE_DIR}/hamstertrash)
endif()

project(hamsterpack)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(source_files
    src/main.cc
    )

include_directories(include)
add_executable(hamsterpack ${source_files})

set_target_properties(hamsterpack
    PROPERTIES
    CMAKE_BUILD_TYPE RELEASE)

if(parentproject)
    #Create dummy source file
    if(NOT EXISTS(${HAM_OUT_FILE}))
        FILE(REMOVE ${HAM_OUT_DIR}/hamsterdata.cc.zip)
        FILE(WRITE ${HAM_OUT_FILE})
    endif()

    #Adds command that generates hamsterpack header file automatically
    add_custom_target(autopacker ALL
        COMMAND hamsterpack ${HAM_IN_DIR} ${HAM_OUT_FILE}
        DEPENDS hamsterpack
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Packing resources..."
        VERBATIM)
    add_dependencies(${parentproject} autopacker)

    add_library(hamsterdatalib STATIC EXCLUDE_FROM_ALL ${HAM_OUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/src/hamsterpack.cc)
    set_target_properties(hamsterdatalib PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_dependencies(hamsterdatalib autopacker)

    set(HAMSTER_LIBS hamsterdatalib PARENT_SCOPE)
    set(HAMSTER_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
endif()
