//
// Created by berg on 04/02/2022.
//

#include <algorithm>
#include <cmath>
#include <set>
#include "../header/SuffixArray.h"

SuffixArray::SuffixArray(int editDistanceThreshold) {
    this->editDistanceThreshold = editDistanceThreshold;
    this->s = 1;
}

SuffixArray::~SuffixArray() {

}

bool lexicographicalOrder(pair<unsigned, unsigned short>& itemA, pair<unsigned, unsigned short>& itemB) {
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

int binarySearchRight(vector<pair<unsigned, unsigned short>>& suffixArray, int begin, int end, const string& prefix) {
    int pivot;
    int result;

    while (begin <= end) {
        pivot = (begin + end) / 2;
        result = prefix.compare(0,
                                prefix.size(),
                                records[suffixArray[pivot].first].c_str() + suffixArray[pivot].second,
                                0,
                                prefix.size());
        if (result > 0) {
            begin = pivot + 1;
        } else if (result <= 0) {
            end = pivot - 1;
        }
    }
    if (begin < suffixArray.size()) {
        if (prefix.compare(0, prefix.size(), records[suffixArray[begin].first].c_str() + suffixArray[begin].second, 0, prefix.size()) == 0) {
            return begin;
        }
    }
    return -1;
}

int binarySearchLeft(vector<pair<unsigned, unsigned short>>& suffixArray, int begin, int end, const string& prefix) {
    int pivot;
    int result;
    while (begin <= end) {
        pivot = (begin+end) / 2;
        result = prefix.compare(0, prefix.size(), records[suffixArray[pivot].first].c_str() + suffixArray[pivot].second,0, prefix.size());

        if (result >= 0) {
            begin= pivot + 1;
        }
        else if (result < 0) {
            end = pivot - 1;
        }
    }
    if (end > 0) {
        if (prefix.compare(0, prefix.size(), records[suffixArray[end].first].c_str() + suffixArray[end].second,0, prefix.size()) == 0) {
            return end;
        }
    }
    return -1;
}

pair<int, int> SuffixArray::search(const string& prefix) {
    int begin = 0;
    int end = this->suffixes.size() - 1;

    int beginRangeResult = binarySearchRight(this->suffixes, begin, end, prefix);
    int endRangeResult = -1;
    if (beginRangeResult != -1) {
        endRangeResult = binarySearchLeft(this->suffixes, begin, end, prefix);
    }

    return make_pair(beginRangeResult, endRangeResult);
}

vector<string> SuffixArray::exactSearch(const string& prefix) {
  pair<int, int> rangeResults = this->search(prefix);
  vector<string> results = this->fetching(rangeResults.first, rangeResults.second);
  return results;
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

void generateNGram(int n, const string &prefix, vector<string>& ngrams) {
    int i = 0;

    while (i < prefix.size()) {
        ngrams.push_back(prefix.substr(i, n));
        i += n;
    }
}

int calculateEdiDistance(const string& a, const string& b, int candidateSize) {
    int m[a.size() + 1][candidateSize + 1];

    for (int i = 0; i <= a.size(); i++) {
        for (int j = 0; j <= candidateSize; j++) {
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

    return m[a.size()][candidateSize];
}

void SuffixArray::approximateSearch(const string &prefix, unordered_map<int, int>& resultsMap) {
    int gramSize = ceil(float(prefix.size()) / float(this->editDistanceThreshold + this->s));

//    cout << "Generating ngram to prefix..." << endl;
//    auto start = chrono::high_resolution_clock::now();
    vector<string> prefixNGrams;
    generateNGram(gramSize, prefix, prefixNGrams);
//    auto done = chrono::high_resolution_clock::now();
//    cout << "<<<generateNGram(gramSize, prefix, prefixNGrams): "<< chrono::duration_cast<chrono::nanoseconds>(done - start).count() << " ns>>>\n";

    int ngramPosition = 0;
    for (const string& ngram : prefixNGrams) {
//        cout << "Exact searching to ngram..." << endl;
//        start = chrono::high_resolution_clock::now();
        pair<int, int> rangeResult = this->search(ngram);
//        done = chrono::high_resolution_clock::now();
//        cout << "<<<this->search(ngram): "<< chrono::duration_cast<chrono::nanoseconds>(done - start).count() << " ns>>>\n";

        if (rangeResult.first != -1 && rangeResult.second != -1) {
//            cout << "Generating candidates to ngram..." << endl;
//            start = chrono::high_resolution_clock::now();

            for (int i = rangeResult.first; i <= rangeResult.second; i++) {
                pair<int, int> item = this->suffixes[i];
                int candidateSize;

                if (resultsMap.find(item.first) == resultsMap.end()) {
                    if (item.second >= ngramPosition - this->editDistanceThreshold &&
                        item.second <= ngramPosition + this->editDistanceThreshold) {

                        candidateSize = records[item.first].size();
                        if (candidateSize > prefix.size()) {
                            candidateSize = prefix.size();
                        }

                        if (candidateSize > prefix.size() - this->editDistanceThreshold &&
                            calculateEdiDistance(prefix, records[item.first], candidateSize) <= this->editDistanceThreshold) {
                            resultsMap[item.first] = 1;
                        }
                    }
                }
            }
//            done = chrono::high_resolution_clock::now();
//            cout << "<<<candidates time: "<< chrono::duration_cast<chrono::nanoseconds>(done - start).count() << " ns>>>\n";
        }

//        cout << "\n" << endl;

        ngramPosition += ngram.size();
    }
}
