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
#include "../header/utils.h"

using namespace std;

Experiment* experiment;
vector<string> records;

Framework::Framework() {
    this->editDistanceThreshold = stoi(config["edit_distance"]);
    this->dataset = stoi(config["dataset"]);
    experiment = new Experiment(config, stoi(config["edit_distance"]));

    index();
}

Framework::~Framework() {
    cout << "deleting framework" << endl;
    delete this->suffixArray;
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

void Framework::readData(string& filename, vector<string>& recs, bool insertEndOfWord) {
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
        if (insertEndOfWord) str += "$";
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
            break;
        default:
            datasetFile = "/home/berg/workspace/mestrado/fuzzy-suffix-array/test.txt";
            queryFile = "/home/berg/workspace/mestrado/fuzzy-suffix-array/q.txt";
            break;
    }

    readData(datasetFile, records, true);
    //    sort(this->records.begin(), this->records.end());
    readData(queryFile, this->queries);

    this->suffixArray = new SuffixArray(this->editDistanceThreshold);
    this->suffixArray->build();

    auto done = chrono::high_resolution_clock::now();

    #ifdef BEVA_IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInIndexing();
    #else
        experiment->endIndexingTime();
    #endif
    cout << "<<<Index time: "<< chrono::duration_cast<chrono::milliseconds>(done - start).count() << " ms>>>\n";

//    string prefix = "ba";
//    cout << "Searching in array to prefix: " << prefix << endl;
//    vector<string> results = this->suffixArray->exactSearch(prefix);
//    for (auto & result : results) {
//        cout << "Result: " << result << endl;
//    }

//    string prefix = "volvo";
//    cout << "Searching in array to prefix: " << prefix << endl;
//
//    start = chrono::high_resolution_clock::now();
//    unordered_map<int, int> resultsMap;
//    this->suffixArray->approximateSearch(prefix, resultsMap);
//    for (const auto& result : resultsMap) {
//        cout << "result: " << records[result.first] << endl;
//    }
//    vector<string> results = this->suffixArray->exactSearch(prefix);
//    for (const string& result : results) {
//        cout << "result: " << result << endl;
//    }
    done = chrono::high_resolution_clock::now();
    cout << "<<<Processing time: "<< chrono::duration_cast<chrono::milliseconds>(done - start).count() << " ms>>>\n";
}

vector<string> Framework::processQuery(string &query, int queryId) {
    vector<string> results;
    vector<int> prefixQuerySizes = {5, 9, 13, 17};
    string prefix = query;

    for (int prefixQuerySize : prefixQuerySizes) {
        prefix = query.substr(0, prefixQuerySize);

        if (prefix.size() == prefixQuerySize) {
            #ifdef BEVA_IS_COLLECT_TIME_H
                experiment->initQueryProcessingTime();
            #endif
            unordered_map<int, int> resultsMap;
            this->suffixArray->approximateSearch(prefix, resultsMap);

            #ifdef BEVA_IS_COLLECT_TIME_H
                experiment->endQueryProcessingTime(prefixQuerySize);
                experiment->initQueryFetchingTime();
                results = output(resultsMap);
                experiment->endQueryFetchingTime(prefixQuerySize, results.size());
            #endif
        }
    }

    #ifdef BEVA_IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInProcessing();
    #else
        experiment->compileQueryProcessingTimes(queryId);
        experiment->saveQueryProcessingTime(prefix, queryId);
    #endif

    return results;
}

vector<string> Framework::processFullQuery(string &query) {
    #ifdef BEVA_IS_COLLECT_TIME_H
        experiment->initQueryProcessingTime();
    #endif

    unordered_map<int, int> resultsMap;
    this->suffixArray->approximateSearch(query, resultsMap);

    #ifdef BEVA_IS_COLLECT_TIME_H
        experiment->endSimpleQueryProcessingTime();
        experiment->initQueryFetchingTime();
    #endif

    vector<string> results = this->output(resultsMap);

    #ifdef BEVA_IS_COLLECT_TIME_H
        experiment->endSimpleQueryFetchingTime(results.size());
        experiment->compileSimpleQueryProcessingTimes(query);
    #endif

    #ifdef BEVA_IS_COLLECT_MEMORY_H
        experiment->getMemoryUsedInProcessing();
    #endif

    return results;
}

vector<string> Framework::output(const unordered_map<int, int>& resultsMap) {
    vector<string> results;
    results.reserve(resultsMap.size());

    for (const auto& result : resultsMap) {
        results.emplace_back(records[result.first].c_str());
    }

    return results;
}
