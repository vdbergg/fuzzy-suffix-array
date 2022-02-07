//
// Created by berg on 04/02/2022.
//

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXTREENODE_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXTREENODE_H

#include "StaticString.h"


class SuffixTreeNode {
public:
    SuffixTreeNode();
    ~SuffixTreeNode();

    string sub;
    vector<int> child;
};


#endif //FUZZY_SUFFIX_ARRAY_SUFFIXTREENODE_H
