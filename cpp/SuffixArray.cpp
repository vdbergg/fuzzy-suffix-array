//
// Created by berg on 04/02/2022.
//

#include <algorithm>
#include <cmath>
#include <set>
#include "../header/SuffixArray.h"

SuffixArray::SuffixArray() {

}

SuffixArray::~SuffixArray() {

}

bool lexicographicalOrder(pair<unsigned, unsigned short> itemA, pair<unsigned, unsigned short> itemB) {
    return (records[itemA.first].substr(itemA.second) < records[itemB.first].substr(itemB.second));
}

void SuffixArray::build() {
    cout << "Adding pair to array..." << endl;
    for (unsigned recordId = 0; recordId < records.size(); recordId++) {
        for (unsigned short i = 0; i < records[recordId].size(); i++) {
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
        string strPivot = records[suffixArray[pivot].first].substr(suffixArray[pivot].second);
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

vector<string> SuffixArray::exactSearch(const string& prefix) {
  pair<int, int> rangeResults = this->search(prefix);
  vector<string> results = this->fetching(rangeResults.first, rangeResults.second);
  return results;
}

pair<int, int> SuffixArray::search(const string& prefix) {
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
    if (beginRangeResult != -1) {
        begin = beginRangeResult + 1;
        end = this->suffixes.size() - 1;

        do {
            pivot = binarySearch(this->suffixes, begin, end, prefix);
            if (pivot != -1) {
                endRangeResult = pivot;
                begin = pivot + 1;
            }
        } while (pivot != -1);
    }

    return make_pair(beginRangeResult, endRangeResult);
}

vector<string> SuffixArray::fetching(int beginRangeResult, int endRangeResult) {
    set<int> resultsRecordIds;
//    cout << "beginRangeResult: " << beginRangeResult << " endRangeResult: " << endRangeResult << endl;
    for (int i = beginRangeResult; i <= endRangeResult; i++) {
        resultsRecordIds.insert(this->suffixes[i].first);
//        cout << "Index: " << i << " Result: " << records[this->suffixes[i].first].c_str() << " recordId: " << this->suffixes[i].first << " position: " << this->suffixes[i].second << endl;
    }

    vector<string> results;
    for (auto itr = resultsRecordIds.rbegin(); itr != resultsRecordIds.rend(); itr++) {
        results.push_back(records[*itr]);
    }

    return results;
}

vector<string> generateNGram(int n, const string &prefix) {
    vector<string> ngrams;
    int i = 0;

    while (i < prefix.size()) {
        ngrams.push_back(prefix.substr(i, n));
        i += n;
    }

    return ngrams;
}

int calculateEdiDistance(const string& a, const string& b) {
    int m[a.size() + 1][b.size() + 1];

    for (int i = 0; i <= a.size(); i++) {
        for (int j = 0; j <= b.size(); j++) {
            if (i == 0) {
                m[i][j] = j;
            } else if (j == 0) {
                m[i][j] = i;
            } else if (a[i - 1] == b[j - 1]) {
                m[i][j] = m[i - 1][j - 1];
            } else {
                m[i][j] = 1 + utils::min(m[i][j - 1], // Insert
                                m[i - 1][j], // Remove
                                m[i - 1][j - 1]); // Replace
            }
        }
    }

    return m[a.size()][b.size()];
}

unordered_map<int, string> SuffixArray::approximateSearch(const string &prefix) {
    unordered_map<int, string> resultsMap;

    int k = 2;
    int s = 1;
    int nGram = ceil(float(prefix.size()) / float(k + s));

    vector<string> prefixNGrams = generateNGram(nGram, prefix);

    int ngramPosition = 0;
    for (const string& ngram : prefixNGrams) {
        pair<int, int> rangeResult = this->search(ngram);

        unordered_map<int, int> candidatesMapIds;
        for (int i = rangeResult.first; i <= rangeResult.second; i++) {
            pair<int, int> item = this->suffixes[i];

            if (candidatesMapIds.find(item.first) == candidatesMapIds.end()) {
                if (item.second >= ngramPosition - k && item.second <= ngramPosition + k) {
                    candidatesMapIds[item.first] = 1;
                }
            }
        }

//        cout << "ngram: " << ngram << endl;
        for (auto item : candidatesMapIds) {
            string candidate = records[item.first];
//            cout << "Candidate: " << candidate << endl;
            if (candidate.size() > prefix.size()) {
                candidate = candidate.substr(0, prefix.size());
            }

//            cout << "Candidate cortado: " << candidate << endl;
//            cout << "Prefix: " << prefix << endl;

            if (resultsMap.find(item.first) == resultsMap.end()) {
                if (candidate.size() > prefix.size() - k && calculateEdiDistance(prefix, candidate) <= k) {
                    resultsMap[item.first] = records[item.first];
                }
            }
        }

        ngramPosition += ngram.size();
    }

  return resultsMap;
}
