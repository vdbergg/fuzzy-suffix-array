//
// Created by berg on 04/02/2022.
//

#include "../header/SuffixTree.h"

SuffixTree::SuffixTree() {
    this->nodes.push_back(new SuffixTreeNode());
}

SuffixTree::~SuffixTree() {

}

void SuffixTree::build() {
    for (int recordId = 0; recordId < records.size(); recordId++) {
        string record = "banana";
        record += "$";
//        string record = "cattcat$";

        for (int i = 0; i < record.size(); ++i) {
            string suffix = record.substr(i);
            this->addSuffix(suffix);
        }
        break;
    }
}

void SuffixTree::addSuffix(string &suffix) {
    // Declare some useful auxiliary variables
    int n = 0;
    int i = 0;
    int x2 = 0;
    int n2 = 0;
    int n3 = 0;
    int j = 0;
    SuffixTreeNode *temp = nullptr;
    bool process = true;
    string sub2;

    while (i < suffix.length()) {
        char b = suffix[i];

        while (process) {
            if (x2 == this->nodes[n]->child.size()) {
                n2 = this->nodes.size();
                temp = new SuffixTreeNode();
                temp->sub = suffix.substr(i);


                this->nodes.push_back(temp);
                this->nodes[n]->child.push_back(n2);
                return;
            }

            n2 = this->nodes[n]->child.at(x2);
            if (this->nodes.at(n2)->sub[0] == b) {
                process = false;
            } else {
                x2++;
            }
        }
        sub2 = this->nodes.at(n2)->sub;
        process = true;
        while (j < sub2.length() && (i + j) < suffix.length() && process) {
            if (suffix[i + j] != sub2[j]) {
                n3 = n2;
                n2 = this->nodes.size();
                temp = new SuffixTreeNode();
                temp->sub = sub2.substr(0, j);
                temp->child.push_back(n3);
                this->nodes.push_back(temp);
                this->nodes.at(n3)->sub = sub2.substr(j);
                this->nodes.at(n)->child.at(x2) = n2;
                process = false;
            } else {
                j++;
            }
        }

        i += j;
        n = n2;
        // Reset value
        x2 = 0;
        n2 = 0;
        n3 = 0;
        j = 0;
        temp = nullptr;
        process = true;
        sub2 = "";
    }
}

void SuffixTree::printData(int n, const string& prefix) {
    vector<int> children = this->nodes[n]->child;
    if (children.empty()) {
        cout << "⤑ " << this->nodes.at(n)->sub << endl;
        return;
    }

    cout << "┐ " << this->nodes.at(n)->sub << endl;
    for (int i = 0; i < children.size() - 1; i++) {
        int c = children.at(i);
        cout << prefix << "├─";
        this->printData(c, prefix  +  "│ ");
    }

    cout << prefix << "└─";
    this->printData(children.at(children.size() - 1), prefix  +  "  ");
}

void SuffixTree::visualize() {
    if (this->nodes.empty()) {
        cout << "\nEmpty Tree";
        return;
    }
    this->printData(0, "");
}

void SuffixTree::depthFirstTraversal() {

}
