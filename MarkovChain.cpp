#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>

using namespace std;

// for MarkovChain::train
// text 에서 sIdx 에서 eIdx 까지(eIdx 도 포함.)
string getTrainTokenString(vector<string> text, int sIdx, int eIdx) {
    string result;
    for (int i=sIdx; i<=eIdx; i++) {
        result += text[i];
        if (i != eIdx) result += " ";
    }

    return result;
}

class MarkovChain {
public :

    int order;
    unordered_map<string, vector<string>> table;
    mt19937 rng;

    MarkovChain(int order = 2) : order(order), rng(random_device{}()) {} 

    vector<string> tokenize(const string& text) {
        istringstream ss(text);
        vector<string> result;
        string word;
        while (ss >> word) {
            result.push_back(word);
        }

        return result;
    }

    void train(const string& text) {
        vector<string> words = tokenize(text);

        // order 는 고정값. 
        for (int i = 0; i < words.size()-order; i++) {
            string token = getTrainTokenString(words, i, i+order-1);
            table[token].push_back(words[i+order]);
        }

        return;
    }

    string generate(int max_words_count = 20) {
        if (table.empty()) return "";

        string key;
        auto it = table.begin();
        advance(it, rng() % table.size());
        key = it->first;

        vector<string> result = tokenize(key);

        for (int i=0; i<max_words_count; i++) {
            
            // nexts (vector<string>) 을 만들어서, 다음에 들어올 수 있는 단어들을 넣어두기
            vector<string> nexts = table[key];

            // rng() 를 이용하여, 랜덤으로 선택 후 결과값에 추가.
            string next = nexts[rng() % nexts.size()];
            result.push_back(next);

            // key 를 현재 고른 단어에서 다음 단어로 바꾸어줘야함??
            vector<string> tmp = tokenize(key);
            tmp.erase(tmp.begin());
            tmp.push_back(next);
            key = getTrainTokenString(tmp, 0, tmp.size()-1);

            // 탈출 조건?
            if (table.find(key) == table.end()) break;
        }

        return getTrainTokenString(result, 0, result.size()-1);
    }
};

int main() {
    MarkovChain mc(2);

    cout << "학습할 텍스트를 입력하세요 (끝나면 빈 줄 엔터):\n";

    string line, corpus;
    while (getline(cin, line)) {
        if (line.empty()) break;
        corpus += line + " ";
    }

    mc.train(corpus);

    cout << "\n생성 결과:\n";
    cout << mc.generate(30) << "\n";
}
