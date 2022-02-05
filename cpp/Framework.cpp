//
// Created by vdberg on 12/02/19.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <sys/stat.h>
#include "../header/C.h"
#include "../header/Framework.h"
#include "../header/Directives.h"

using namespace std;

Experiment* experiment;
vector<StaticString> records;

Framework::Framework() {
    this->editDistanceThreshold = stoi(config["edit_distance"]);
    this->dataset = stoi(config["dataset"]);
    experiment = new Experiment(config, stoi(config["edit_distance"]));

    index();
}

Framework::~Framework() {
    cout << "deleting framework" << endl;
    delete this->suffixTree;
}

unsigned long getFileSize(string filename) {
    FILE *fp=fopen(filename.c_str(),"r");

    struct stat buf;
    fstat(fileno(fp), &buf);
    fclose(fp);
    return buf.st_size;
}

void Framework::readData(string& filename, vector<StaticString>& recs) {
    cout << "reading dataset " << filename << endl;

    string str;
    ifstream input(filename, ios::in);

    unsigned long fileSize = getFileSize(filename);
//    cout << "Tamanho do Arquivo:" << fileSize << endl;
    char *tmpPtr = (char*) malloc(sizeof(char)*fileSize);
    StaticString::setDataBaseMemory(tmpPtr,fileSize);
    while (getline(input, str)) {
//        for (char &c : str) {
//            if ((int) c == -61) continue;
//            else if ((int) c < 0 || (int) c >= CHAR_SIZE) {
//                c = utils::convertSpecialCharToSimpleChar(c);
//            }
//            c = tolower(c);
//        }
        if (!str.empty()) recs.push_back(StaticString(str));
    }
}

void Framework::readData(string& filename, vector<string>& recs) {
    cout << "reading dataset " << filename << endl;

    string str;
    ifstream input(filename, ios::in);
    while (getline(input, str)) {
//        for (char &c : str) {
//            if ((int) c == -61) continue;
//            else if ((int) c < 0 || (int) c >= CHAR_SIZE) {
//                c = utils::convertSpecialCharToSimpleChar(c);
//            }
//            c = tolower(c);
//        }
        if (!str.empty()) recs.push_back(str);
    }
}

void Framework::index(){
    cout << "indexing... \n";
    string sizeSufix = "";
    switch (stoi(config["size_type"])) {
        case 0:
            sizeSufix = "_25";
            break;
        case 1:
            sizeSufix = "_50";
            break;
        case 2:
            sizeSufix = "_75";
            break;
        case 3:
            sizeSufix = "";
            break;
        default:
            sizeSufix = "_60";
    }

    auto start = chrono::high_resolution_clock::now();

    #ifdef BEVA_IS_COLLECT_TIME_H
        experiment->initIndexingTime();
    #endif
    
    string datasetFile = config["dataset_basepath"];
    string queryFile = config["query_basepath"];
    string relevantQueryFile = config["query_basepath"];

    int queriesSize = stoi(config["queries_size"]);
    string datasetSuffix = queriesSize == 10 ? "_10" : "";
    string tau = to_string(this->editDistanceThreshold);

    switch (this->dataset) {
        case C::AOL:
            datasetFile += "aol/aol" + sizeSufix + ".txt";
            queryFile += "aol/q17_" + tau + datasetSuffix + ".txt";
            break;
        case C::MEDLINE:
            datasetFile += "medline/medline" + sizeSufix + ".txt";
            queryFile += "medline/q13" + datasetSuffix + ".txt";
            break;
        case C::USADDR:
            datasetFile += "usaddr/usaddr" + sizeSufix + ".txt";
            queryFile += "usaddr/q17_" + tau + datasetSuffix + ".txt";
            break;
        case C::MEDLINE19:
            datasetFile += "medline19/medline19" + sizeSufix + ".txt";
            queryFile += "medline19/q17_" + tau + datasetSuffix + ".txt";
            break;
        case C::DBLP:
            datasetFile += "dblp/dblp" + sizeSufix + ".txt";
            queryFile += "dblp/q17_" + tau + datasetSuffix + ".txt";
	    break;
        case C::UMBC:
            datasetFile += "umbc/umbc" + sizeSufix + ".txt";
            queryFile += "umbc/q17_" + tau + datasetSuffix + ".txt";
            break;
        case C::JUSBRASIL:
            datasetFile += "jusbrasil/jusbrasil" + sizeSufix + ".txt";
            queryFile += "jusbrasil/q.txt";
            relevantQueryFile += "jusbrasil/relevant_answers.txt";
            break;
        default:
            datasetFile += "aol/aol" + sizeSufix + ".txt";
            queryFile += "aol/q17_" + tau + datasetSuffix + ".txt";
            break;
    }

    readData(datasetFile, records);
    //    sort(this->records.begin(), this->records.end());
    readData(queryFile, this->queries);
    if (config["has_relevant_queries"] == "1") {
        readData(relevantQueryFile, this->relevantQueries);
    }

//    this->suffixTree = new SuffixTree();
//    this->suffixTree->build();
//    this->suffixTree->visualize();
    this->arraySuffix = new SuffixArray();
    this->arraySuffix->build();

    cout << "Searching test" << endl;
    string prefix = "ab";
    pair<int, int> result = this->arraySuffix->search(prefix);
    cout << "beginRangeResult: " << result.first << " endRangeResult: " << result.second << endl;
    for (int index = result.first; index < result.second; index++) {
        string strResult = records[this->arraySuffix->suffixes[index]->recordId].c_str();
        strResult = strResult.substr(this->arraySuffix->suffixes[index]->position);
        cout << "Result: " << strResult << endl;
    }

//    this->beva = new Beva(this->trie, this->editDistanceThreshold);

    auto done = chrono::high_resolution_clock::now();

    #ifdef BEVA_IS_COLLECT_MEMORY_H
        this->experiment->getMemoryUsedInIndexing();
    #else
        experiment->endIndexingTime();
        experiment->compileProportionOfBranchingSizeInBEVA2Level();
        experiment->compileNumberOfNodes();
    #endif
    cout << "<<<Index time: "<< chrono::duration_cast<chrono::milliseconds>(done - start).count() << " ms>>>\n";
}
