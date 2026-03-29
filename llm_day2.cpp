#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

// 여기서부터 Vector

class Vector {
public:
    std::vector<double> data;

    // 백터 생성자
    Vector(int size); // size의 크기의 0으로 초기화된 백터
    Vector(const std::vector<double>& d); // 주어진 데이터로 Vector 생성

    // 생성자 선언
    Vector(std::initializer_list<double> list) : data(list) {}

    // 사이즈
    int size() const;

    // 벡터 원소 접근
    double& operator[](int i); // 값 수정
    double operator[](int i) const; // 값 읽기

    // 덧셈 곱셈 오퍼레이터
    Vector operator+(const Vector& other) const;
    Vector operator*(double scalar) const;

    // 벡터 연산
    double dot(const Vector& other) const;
};

// 벡터의 길이 size() 출력하기
int Vector::size() const {
    return data.size();
}

// 크기 기반 생성자
// 주어진 size만큼의 벡터를 만들고 모든 값을 0.0으로 초기화한다.
// ex) Vector(3) → {0.0, 0.0, 0.0}
Vector::Vector(int size) : data(size, 0.0) {};

// 데이터 기반 생성자
// 외부에서 전달받은 std::vector 데이터를 그대로 복사하여 초기화한다.
// ex) Vector({1,2,3}) → {1,2,3}
Vector::Vector(const vector<double>& d) : data(d) {};

// 벡터 원소 접근 (쓰기 가능)
// 범위를 벗어나면 런타임 에러 발생
double& Vector::operator[](int i) {

    // i 인덱스 확인
    if (i < 0 || i >= data.size()) throw runtime_error("Index out of range!!");

    // 값 반환
    return data[i];

};

// 벡터 원소 접근 (읽기 전용)
// const 객체에서도 접근 가능
double Vector::operator[](int i) const {

    // i 인덱스 확인
    if (i < 0 || i >= data.size()) throw runtime_error("Index out of range!!");

    // 값 반환
    return data[i];

};

// 벡터 덧셈
// 같은 크기의 벡터가 아닐 시 런타임 에러 발생
// 결과는 새로운 Vector로 변환
Vector Vector::operator+(const Vector& other) const{

    // 벡터 사이즈가 안맞을시 연산오류 런타임에러
    if (data.size() != other.data.size()) throw runtime_error("Vector size not matching!!");

    // 출력용 데이터 만들기
    Vector result(data.size());

    // 벡터 두개 합치기
    for (int i=0; i<data.size(); i++) {
        result[i] = data[i] + other.data[i];
    }

    // 출력
    return result;
};

// 스칼라 곱
// 벡터 내의 모든 원소에 scalar값만큼 곱함
Vector Vector::operator*(double scalar) const {
    // 출력용 데이터 만들기
    Vector result(data.size());

    // 계산
    for (int i=0; i<data.size(); i++) {
        result[i] = data[i] * scalar;
    }

    // 출력
    return result;
};

// 벡터 내적
// 각 원소를 곱한 뒤 더한 값을 double 값으로 반환
// ex) v1.dot(v2) = 20 | [1, 2, 3] · [1, 5, 10] = 1*2 + 2*5 + 3*10
double Vector::dot(const Vector& other) const {

    // 벡터 사이즈가 안맞을시 연산오류 런타임에러
    if (data.size() != other.data.size()) {
        throw runtime_error("Vector size not matching!!");
    }

    // 출력용 데이터 만들기
    double result = 0;

    // 내적 구하기
    for (int i=0; i<data.size(); i++) {
        result += data[i] * other.data[i];
    }

    // 출력
    return result;
};


// 여기서부터 MATRIX

class Matrix {
public:
    std::vector<std::vector<double>> data;

    // Matrix 생성
    Matrix(int rows, int cols);
    Matrix(std::vector<std::vector<double>> d);

    // 행, 열 크기
    int rows() const;
    int cols() const;

    // Matrix 원소 접근
    std::vector<double>& operator[](int i);
    const std::vector<double>& operator[](int i) const;

    // 벡터 곱
    Vector operator*(const Vector& v) const;
    // 행렬 곱
    Matrix operator*(const Matrix& other) const;

    // 전치 행렬
    Matrix transpose() const;
};

// Matrix 생성자
Matrix::Matrix(std::vector<std::vector<double>> d) : data(d) {}

// rows() 출력하기
int Matrix::rows() const {
    return data.size();
};

// cols() 출력하기
int Matrix::cols() const {
    if (data.empty()) return 0;
    return data[0].size();
};

// 값 수정
std::vector<double>& Matrix::operator[](int i) {
    if (i<0 || i>=rows()) {
        throw runtime_error("Index out of range!!");
    }
    return data[i];
}

// 값 읽기
const std::vector<double>& Matrix::operator[](int i) const {
    if (i<0 || i>=rows()) {
        throw runtime_error("Index out of range!!");
    }
    return data[i];
}

// Matrix 생성
Matrix::Matrix(int rows, int cols)
    : data(rows, std::vector<double>(cols, 0.0)) {}

// Matrix-Vector 연산
Vector Matrix::operator*(const Vector& v) const {

    // 크기 확인
    if (cols() != v.size()) throw runtime_error("Matrix-Vector size not matching!!");

    // 출력 벡터 만들기
    Vector result(rows());

    // dot product
    for (int i=0; i<rows(); i++) {
        double sum = 0;

        for (int j=0; j<cols(); j++) {
            sum += data[i][j] * v[j];
        }

        result[i] = sum;
    }

    // 출력
    return result;
};

//
Matrix Matrix::operator*(const Matrix& other) const {

    if (cols() != other.rows()) throw runtime_error("Matrix-Matrix size not matching!!");

    // 출력 매트릭스 만들기
    Matrix result(rows(), other.cols());

    // 행렬 곱
    for (int i=0; i<rows(); i++) {
        for (int j=0; j<other.cols(); j++) {
            double sum = 0;

            for (int k=0; k<cols(); k++) {
                sum += data[i][k] * other.data[k][j];
            }

            result[i][j] = sum;
        }
    }

    // 출력
    return result;
};




// 아래부터는 프로그램.

void program1() {

    Vector v1({1, 2, 3}), v2({1, 1, 1});

    Vector v3 = v1 * 2;

    for (int i=0; i<3; i++) cout << v3[i] << " ";
    cout << '\n';

    return;

};

void program2() {

    Vector v1({1, 2, 3}), v2({2, 1, 5});

    double t = v1.dot(v2);

    cout << t;

    return;

};

void program3() {
    Matrix A({{1,2,3,4},{5,6,7,8}}), B({{1,2,3},{3,4,5},{5,6,7},{7,8,9}});

    Matrix C = A * B;

    for (int i=0; i<C.rows(); i++) {
        cout << "|  ";
        for (int j=0; j<C.cols(); j++) {
            cout << C[i][j] << " ";
        }
        cout << " |\n";
    }

    return;
}


// 여기서부터는 메인

int main() {
    
    cout << "[START]==============\n\n";

    program3();

    while (true) {
        string tmp;
        cout << "\n\n================[END]\nType 'n' to end\n";
        getline(cin, tmp);
        if (tmp == "n") break;
    }
    return 0;
};
