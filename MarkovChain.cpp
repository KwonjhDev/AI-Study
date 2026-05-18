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
        string key;
        auto it = table.begin();
        advance(it, rng() % table.size());
        key = it->first;

        vector<string> result = tokenize(key);

        for (int i=0; i<max_words_count; i++) {
            vector<string> str = table[key];
            result.push_back(" " + str[rng() % str.size()]);
            if (table.find(key) == table.end()) break;
        }

        return getTrainTokenString(result, 0, result.size()-1);
    }
    
};
