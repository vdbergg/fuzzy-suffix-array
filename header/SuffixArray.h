//
// Created by berg on 04/02/2022.
//

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H

#include "StaticString.h"
#include "GlobalVariables.h"

class SuffixArray {
public:
    SuffixArray();
    ~SuffixArray();

    void build();
    vector<string> search(const string& prefix);
    vector<string> fetching(pair<int, int> rangeResults);

    vector<pair<unsigned, unsigned short>> suffixes;
};


#endif //FUZZY_SUFFIX_ARRAY_SUFFIXARRAY_H
