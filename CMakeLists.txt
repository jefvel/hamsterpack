set(parentproject ${PROJECT_NAME})

if(parentproject)
    set(HAM_OUT_FILE ${PROJECT_SOURCE_DIR}/hamsterfile.cc)
endif()

project(hamsterpack)
cmake_minimum_required(VERSION 3.0.2)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(source_files
    src/main.cc
    )

include_directories(include)
add_executable(hamsterpack ${source_files})

if(parentproject)
    #Create dummy source file
    FILE(WRITE ${HAM_OUT_FILE})
    #Adds command that generates hamsterpack header file automatically
    add_custom_target(autopacker ALL
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/hamsterpack ${HAM_IN_DIR} ${HAM_OUT_FILE}
        DEPENDS hamsterpack
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Packing resources..."
        VERBATIM)

    add_dependencies(${parentproject} autopacker)
endif()
