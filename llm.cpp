#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class Vector {
public:
    std::vector<double> data;

    // 백터 생성자
    Vector(int size); // size의 크기의 0으로 초기화된 백터
    Vector(std::vector<double> d); // 주어진 데이터로 Vector 생성

    // 사이즈
    int size() const;

    // 백터 원소 접근
    double& operator[](int i); // 값 수정
    double operator[](int i) const; // 값 읽기

    // 덧셈 곱셈 오퍼레이터
    Vector operator+(const Vector& other) const;
    Vector operator*(double scalar) const;

    // 백터 연산
    double dot(const Vector& other) const;
};

Vector::Vector(int size) {
    data = vector<double>(size, 0.0);
};

Vector::Vector(vector<double> d) {
    data = d;
};

// 값 수정
double& Vector::operator[](int i) {

    return data[i];

};

// 값 읽기
double Vector::operator[](int i) const {

    return data[i];

};

Vector Vector::operator+(const Vector& other) const{

    // 백터 사이즈가 안맞을시 연산오류 런타임에러
    if (data.size() != other.data.size()) throw runtime_error("Vector size not matching!!");

    // 출력용 데이터 만들기
    Vector result(data.size());

    // 백터 두개 합치기
    for (int i=0; i<data.size(); i++) {
        result[i] = data[i] + other.data[i];
    }

    // 출력
    return result;
};

Vector Vector::operator*(double scalar) const {

    // 백터 사이즈가 안맞을시 연산오류 런타임에러

    // 출력용 데이터 만들기
    Vector result(data.size());

    // 계산

    // 출력
    return result;
};

int main() {
    Vector v1({1, 2, 3}), v2({4, 5, 6});

    Vector v3 = v1 + v2;

    for (int i=0; i<3; i++) cout << v3[i] << " ";
    cout << '\n';


    while (true) {
        string tmp;
        cout << "Type 'n' to end\n";
        getline(cin, tmp);
        if (tmp == "n") break;
    }
    return 0;
};
