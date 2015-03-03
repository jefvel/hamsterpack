project(hamstertest)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")


#add_definitions(-Os)
add_executable(hamstertest main.cc)

set(HAM_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

#Add hamsterpack stuff
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. trash)
include_directories(${HAMSTER_INCLUDE_FILES})
target_link_libraries(hamstertest ${HAMSTER_LIBS})

#find_package(HamsterPack)
