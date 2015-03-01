project(hamstertest)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")


#add_definitions(-Os)
add_executable(hamstertest main.cc hamsterfile.cc)

#Add hamsterpack stuff
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(HAM_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
find_package(HamsterPack)
