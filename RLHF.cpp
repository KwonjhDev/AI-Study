#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <windows.h>

using namespace std;

// 조정값 변수

const int MAX_ROUND = 1000 // 전체 시도 횟수
const string question = "오늘 점심으로 먹기 좋은게 뭐가 있을까?"; // 질문

struct Answer {
    string text;
    double score;
};

// 아래 백터는 {"답", 점수} 서순.
vector<Answer> answers = {
        {"학식", 0.5},
        {"도시락", 0.5},
        {"라면", 0.5},
        {"삼각김밥", 0.5},
        {"이온음료", 0.5},
        {"철근", 0.5}
};

// 이 아래부터는 코드

pair<int, int> pickTwo(int size) {
    int a = rand() % size;
    int b = rand() % size;
    while (b == a) b = rand() % size;
    return make_pair(a, b);
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    srand(time(0));

    bool autoMode = false;
    string input;

    cout << "질문: " << question << "\n";
    cout << "직접 선택하려면 1 또는 2 입력\n";
    cout << "자동 실행: Run_AI 입력\n\n";

    int round = 1;
    
    while (round <= MAX_ROUND) {
        cout << "=== Round " << round << " ===\n";

        pair<int, int> p = pickTwo(answers.size());
        int i = p.first;
        int j = p.second;

        Answer &a = answers[i];
        Answer &b = answers[j];

        cout << "1: " << a.text << " vs 2: " << b.text << "\n";

        int winner = 0;

        if (!autoMode) {
            cout << "선택 ( 1 / 2 / Run_AI ) : ";
            cin >> input;

            if (input == "Run_AI") {
                autoMode = true;
                cout << "[AI MOD ACTIVATED]\n\n";
                continue;
            } 
            else if (input == "1") {
                winner = 1;
            } 
            else if (input == "2") {
                winner = 2;
            } 
            else {
                cout << "다시 입력해주세요.\n\n";
                continue;
            }
        }

        if (autoMode) {
            double total = a.score + b.score;
            double r = (double)rand() / RAND_MAX;

            if (r < a.score / total) winner = 1;
            else winner = 2;

            cout << "AI 선택: " 
                 << (winner == 1 ? a.text : b.text) << "\n";
        }

        if (winner == 1) {
            a.score += 0.05;
            b.score -= 0.02;
        } else {
            b.score += 0.05;
            a.score -= 0.02;
        }

        for (auto &ans : answers) {
            ans.score = max(0.0, ans.score);
        }

        Answer best = answers[0];
        for (auto &ans : answers) {
            if (ans.score > best.score)
                best = ans;
        }

        cout << "현재 점수:\n";
        for (auto &ans : answers) {
            cout << ans.text << " : " << ans.score << "\n";
        }

        cout << "현재 최고 선택 : " << best.text << "\n\n";

        round++;
    }

    sort(answers.begin(), answers.end(), [](Answer a, Answer b) {
    return a.score > b.score;
    });

    cout << "=== 최종 결과 ===\n";
    for (auto &ans : answers) {
        cout << ans.text << " : " << ans.score << "\n";
    }

    cout << "\nType 'end' to end.";

    while(true) {
        cin >> input;
        if(input == "end") return 0;
    }
}
