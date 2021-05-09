#include <iostream>
#include <map>
#include <algorithm>
#include "BPT.hpp"
#define TESTSIZE 100000
using namespace RA;
void init() {
    ofstream o("test_tree");
    o.close();
    o.open("test_data");
    o.close();
}

void testIntAll() {
    map<int, vector<int>> answer;
    vector<int> store;
    init();
    srand(22);
    int s;
    BPlusTree<int, int> bpt("test_tree", "test_data");
    for (int i = 0; i < TESTSIZE; i++) {
        s = rand();
        store.push_back(s);
        bpt.insert(s, i);
        if (i % (TESTSIZE / 100) == 0)cout << "[count down] Inserting...\t" << i * 100 / TESTSIZE + 1 << "%" << endl;
    }
    cout << endl;

    for (int i = 0; i < TESTSIZE; i++) {
        if (i % 2 == 0) {
            answer[store[i]].push_back(i);
        }
        if (i % (TESTSIZE / 100) == 0)cout << "[count down] Pushing...\t" << i * 100 / TESTSIZE + 1 << "%" << endl;
    }
    cout << endl;
    int i;
    for (i = 0; i < TESTSIZE; i++) {
        if (i % 2 == 1) {
            if (!bpt.erase(store[i], i)) {
                cerr << "[error]erase failed when erasing\nkey: " << store[i] << "\tdata: " << i << endl;
                break;
            }
        }
        if (i % (TESTSIZE / 100) == 0)cout << "[count down] Erasing...\t" << i * 100 / TESTSIZE + 1 << "%" << endl;
    }
    cout << endl;


    int cnt = 0;
    size_t size = answer.size();
    for (const auto &i:answer) {
        vector<std::pair<int, int>> result;
        result = bpt.find(i.first);
        sort(result.begin(), result.end());
        if (i.second.size() != result.size()) {
            cerr << "[error]wrong size." << endl;
            cerr << "key: " << i.first << endl;
            cerr << "answer size: " << i.second.size() << endl;
            cerr << "answer:" << endl;
            for (auto j : i.second)cerr << j << " ";
            cerr << endl;
            cerr << "result size: " << result.size() << endl;
            cerr << "result:" << endl;
            for (auto j : result)cerr << j.second << " ";
            cerr << endl;
        }
        else {
            for (int j = 0; j < i.second.size(); j++) {
                if (i.second[j] != result[j].second) {
                    cerr << "[error]wrong answer." << endl;
                    cerr << "data: " << i.second[j] << endl;
                    cerr << "your data: " << result[j].second << endl;
                }
            }
        }
        if (size/100 != 0 && cnt % (size / 100) == 0)cout << "[count down] Checking...\t" << cnt * 100 / size + 1 << "%" << endl;
        cnt++;
    }
    cout << "over\n";
    cout << endl;
}
void testRepeatedInsert() {
    init();
    int a = 100;
    BPlusTree<int, int>bpt("test_tree", "test_data");
    for (int i = 0 ; i < TESTSIZE ; ++i) {
        bpt.insert(a, i);
    }
    vector<pair<int, int>> tt;
    tt = bpt.find(a);
    if (tt.size() != TESTSIZE) cerr << "[error] size wrong!\n";
    else {
        sort(tt.begin(), tt.end());
        int cnt = 0;
        for (auto i: tt) {
            if (i.second != cnt)cerr << "[error] element wrong!" << endl;
            cnt++;
        }
    }
}
int main() {
    testRepeatedInsert();
    return 0;
}
