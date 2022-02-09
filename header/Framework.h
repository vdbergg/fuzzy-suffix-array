//
// Created by vdberg on 12/02/19.
//

#ifndef BEVA_FRAMEWORK_H
#define BEVA_FRAMEWORK_H

#include <vector>
#include <string>
#include "Experiment.h"
#include "GlobalVariables.h"
#include "SuffixArray.h"

using namespace std;

class Framework {
public:
    SuffixArray* suffixArray;
    vector<string> queries;
    vector<string> relevantQueries;
    int editDistanceThreshold;
    int dataset;

    Framework();

    void readData(string&, vector<StaticString>&);
    void readData(string&, vector<string>&, bool insertEndOfWord = false);
    void index();

    ~Framework();
};


#endif //BEVA_FRAMEWORK_H
