cmake_minimum_required(VERSION 3.3)
project(bruinbase)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES
#        Bruinbase.h
#        BTreeIndex.cc
#        BTreeIndex.h
#        BTreeNode.cc
#        BTreeNode.h
#        main.cc
#        PageFile.cc
#        PageFile.h
#        RecordFile.cc
#        RecordFile.h)
#
#add_executable(bruinbase ${SOURCE_FILES})

add_custom_target(bruinbase COMMAND make -C /Users/rawrtan/Google\ Drive/School/CS143/projects/vm-shared/2/bruinbase
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})