cmake_minimum_required(VERSION 3.10)
project(fuzzy-suffix-array)

set(CMAKE_CXX_STANDARD 11)

add_executable(fuzzy-suffix-array main.h cpp/utils.cpp header/utils.h cpp/Framework.cpp header/Framework.h header/C.h cpp/Experiment.cpp header/Experiment.h header/Directives.h cpp/StaticString.cpp header/StaticString.h ../../server/crow_all.h header/GlobalVariables.h cpp/SuffixArray.cpp header/SuffixArray.h)
