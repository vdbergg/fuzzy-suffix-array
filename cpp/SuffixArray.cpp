//
// Created by berg on 04/02/2022.
//

#include <algorithm>
#include <set>
#include "../header/SuffixArray.h"

SuffixArray::SuffixArray() {

}

SuffixArray::~SuffixArray() {

}

bool lexicographicalOrder(pair<unsigned, unsigned short> itemA, pair<unsigned, unsigned short> itemB) {
    string strA = records[itemA.first].c_str();
    string strB = records[itemB.first].c_str();
    return (strA.substr(itemA.second) < strB.substr(itemB.second));
}

void SuffixArray::build() {
    cout << "Adding pair to array..." << endl;
    for (unsigned recordId = 0; recordId < records.size(); recordId++) {
        string record = records[recordId].c_str();
        record += '\0';

        for (unsigned short i = 0; i < record.size(); i++) {
            this->suffixes.emplace_back(recordId, i);
        }
    }

//    int index = 0;
//    for (auto suffixArrayItem : this->suffixes) {
//        string str = records[suffixArrayItem.first].c_str();
//        cout << "Index: " << index << " Suffix: " << str.substr(suffixArrayItem.second).c_str() << " recordId: " << suffixArrayItem.first <<  " position: " << suffixArrayItem.second << endl;
//        index++;
//    }

    cout << "Sorting array..." << endl;
    sort(this->suffixes.begin(), this->suffixes.end(), lexicographicalOrder);
//    cout << "suffixes sorted" << endl;
//    index = 0;
//    for (auto suffixArrayItem : suffixes) {
//        string str = records[suffixArrayItem.first].c_str();
//        cout << "Index: " << index << " Suffix: " << str.substr(suffixArrayItem.second) << " recordId: " << suffixArrayItem.first << " position: " << suffixArrayItem.second << endl;
//        index++;
//    }
}

int binarySearch(vector<pair<unsigned, unsigned short>> suffixArray, int begin, int end, const string& prefix) {
    if (end >= begin) {
        int pivot = begin + (end - begin) / 2;
        string strPivot = records[suffixArray[pivot].first].c_str();
        strPivot = strPivot.substr(suffixArray[pivot].second);
        if (strPivot.size() > prefix.size()) {
            strPivot = strPivot.substr(0, prefix.size());
        }

        if (strPivot == prefix) {
            return pivot;
        }

        if (strPivot > prefix) {
            return binarySearch(suffixArray, begin, pivot - 1, prefix);
        }

        return binarySearch(suffixArray, pivot + 1, end, prefix);
    }

    return -1;
}

vector<char*> SuffixArray::search(const string& prefix) {
    int beginRangeResult = -1;
    int endRangeResult = -1;

    // Getting the first prefix match
    int begin = 0;
    int end = this->suffixes.size() - 1;
    int pivot = -1;

    do {
        pivot = binarySearch(this->suffixes, begin, end, prefix);
        if (pivot != -1) {
            beginRangeResult = endRangeResult = pivot;
            end = pivot - 1;
        }
    } while (pivot != -1);

    // Getting the last prefix match
    begin = beginRangeResult + 1;
    end = this->suffixes.size() - 1;

    do {
        pivot = binarySearch(this->suffixes, begin, end, prefix);
        if (pivot != -1) {
            endRangeResult = pivot;
            begin = pivot + 1;
        }
    } while (pivot != -1);

    vector<char*> results = this->fetching(make_pair(beginRangeResult, endRangeResult));

    return results;
}

vector<char*> SuffixArray::fetching(pair<int, int> rangeResults) {
    set<int> resultsRecordIds;

//    cout << "beginRangeResult: " << rangeResults.first << " endRangeResult: " << rangeResults.second << endl;
    if (rangeResults.first != -1 && rangeResults.second != -1) {
        for (int index = rangeResults.first; index <= rangeResults.second; index++) {
            resultsRecordIds.insert(this->suffixes[index].first);
//            cout << "Index: " << index << " Result: " << records[this->suffixes[index].first].c_str() << " recordId: " << this->suffixes[index].first << " position: " << this->suffixes[index].second << endl;
        }
    }

    vector<char*> results;

    for (auto itr = resultsRecordIds.rbegin(); itr != resultsRecordIds.rend(); itr++) {
        results.push_back(records[*itr].c_str());
    }

    return results;
}
