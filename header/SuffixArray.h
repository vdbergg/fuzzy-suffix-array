//
// Created by berg on 04/02/2022.
//

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H

#include "StaticString.h"
#include "GlobalVariables.h"
#include "utils.h"

class SuffixArray {
public:
    SuffixArray();
    ~SuffixArray();

    void build();
    unordered_map<int, string> approximateSearch(const string& prefix);
    vector<string> exactSearch(const string& prefix);
    pair<int, int> search(const string& prefix);
    vector<string> fetching(int beginRangeResult, int endRangeResult);

    vector<pair<unsigned, unsigned short>> suffixes;
};


#endif //FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H
