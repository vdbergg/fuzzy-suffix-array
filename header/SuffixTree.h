//
// Created by berg on 04/02/2022.
//

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXTREE_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXTREE_H

#include "StaticString.h"
#include "SuffixTreeNode.h"
#include "GlobalVariables.h"

class SuffixTree {
public:
    SuffixTree();
    ~SuffixTree();

    void build();
    void addSuffix(string& suffix);
    void printData(int n, const string& prefix);
    void visualize();
    void depthFirstTraversal();

    vector<SuffixTreeNode*> nodes;
};


#endif //FUZZY_SUFFIX_ARRAY_SUFFIXTREE_H
