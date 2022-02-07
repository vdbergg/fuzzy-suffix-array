//
// Created by berg on 04/02/2022.
//

#ifndef FUZZY_SUFFIX_ARRAY_SUFFIXARRAYITEM_H
#define FUZZY_SUFFIX_ARRAY_SUFFIXARRAYITEM_H

#include "StaticString.h"


class SuffixArrayItem {
public:
    SuffixArrayItem(unsigned record, unsigned position);
    ~SuffixArrayItem();

    unsigned recordId;
    unsigned position;
};


#endif //FUZZY_SUFFIX_ARRAY_SUFFIXARRAYITEM_H
