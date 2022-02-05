cmake_minimum_required(VERSION 3.10)
project(fuzzy-suffix-array)

set(CMAKE_CXX_STANDARD 11)

add_executable(fuzzy-suffix-array main.cpp cpp/utils.cpp header/utils.h cpp/Framework.cpp header/Framework.h header/C.h cpp/Experiment.cpp header/Experiment.h header/ShortVector.h header/Directives.h cpp/StaticString.cpp header/StaticString.h header/crow_all.h cpp/SuffixTree.cpp header/SuffixTree.h cpp/SuffixTreeNode.cpp header/SuffixTreeNode.h header/GlobalVariables.h cpp/SuffixArray.cpp header/SuffixArray.h cpp/SuffixArrayItem.cpp header/SuffixArrayItem.h)
