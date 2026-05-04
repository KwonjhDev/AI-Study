#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>

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
// ex) v1.dot(v2) = 20 | [1, 2, 3] · [1, 5, 10] = 1*1 + 2*5 + 3*10
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

Vector softmax(const Vector& v) {

    // 충돌 경우 예외
    if (v.size() == 0) {
        throw runtime_error("Empty Vector!!");
    }

    // 리턴 벡터 생성
    Vector result(v.size());

    // max 찾기
    double max_val = v[0];
    for (int i=0; i<v.size(); i++) {
        if (v[i] > max_val) max_val = v[i];
    }

    // exp 계산 + 합 (max_val을 빼야함.)
    vector<double> exp_vals(v.size()); // exp_vals 을 만들어 계산 수를 줄이기
    double sum = 0;
    for (int i=0; i<v.size(); i++) {
        exp_vals[i] = std::exp(v[i] - max_val);
        sum += exp_vals[i];
    }

    // 정규화
    for (int i=0; i<v.size(); i++) {
        result[i] = exp_vals[i] / sum;
    }

    // 리턴
    return result;
}

// 여기서부터 MATRIX

class Matrix {
public:
    std::vector<std::vector<double>> data;

    // Matrix 생성
    Matrix(int rows, int cols);
    Matrix(const std::vector<std::vector<double>> d);

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

Matrix operator+(const Matrix& A, const Matrix& B);

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

// Matrix 합
Matrix operator+(const Matrix& A, const Matrix& B) {
    if (A.cols() != B.cols() || A.rows() != B.rows()) throw runtime_error("Matrix size not matching!!");

    Matrix result(A.rows(), A.cols()); 

    for (int i=0; i<result.rows(); i++) {
        for (int j=0; j<result.cols(); j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

// Matrix-Matrix 연산
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

// 전치 행렬
Matrix Matrix::transpose() const {

    // 출력 매트릭스  
    Matrix result(cols(), rows());

    // 전치 행렬 프로그램
    for (int i=0; i<rows(); i++) {
        for (int j=0; j<cols(); j++) {
            result[j][i] = data[i][j];
        }
    }

    // 출력
    return result;

}

Matrix softmax(const Matrix& m) {
    
    // Matrix 사이즈 확인
    if (m.rows() < 1 || m.cols() < 1) {
        throw runtime_error("Matrix size ERROR!!");
    }

    // 출력용 매트릭스 생성
    Matrix result(m.rows(), m.cols());

    // 행마다 softmax 돌리기 (vector 저장이라 가능.)
    for (int i=0; i<m.rows(); i++) {
        Vector row(m.cols());

        for (int j=0; j<m.cols(); j++) {
            row[j] = m[i][j];
        }

        Vector v = softmax(row);

        for (int j=0; j<m.cols(); j++) {
            result[i][j] = v[j];
        }
    }

    // 리턴
    return result;
}

// Attention 함수
Matrix attention(const Matrix& Q,
                 const Matrix& K,
                 const Matrix& V) {

    // 1. Q · Kᵀ (유사도 계산) 
    Matrix scores = Q * K.transpose();

    // 2. scaling
    double scale = std::sqrt(Q.cols());
    for (int i=0; i<scores.rows(); i++) {
        for (int j=0; j<scores.cols(); j++) {
            scores[i][j] /= scale;
        }
    }

    // 3. softmax (각 행마다 확률)
    Matrix weights = softmax(scores);

    // 4. weight · V (정보 조합)
    Matrix result = weights * V;

    return result;
}

Matrix multiHeadAttention(const Matrix& Q,
                          const Matrix& K,
                          const Matrix& V,
                          int num_heads) {

    int d = Q.cols();
    int head_dim = d / num_heads;

    int n = Q.rows();

    // 출력용 결과
    Matrix result(n, d);

    for (int h=0; h<num_heads; h++) {
        Matrix Qh(n, head_dim);
        Matrix Kh(n, head_dim);
        Matrix Vh(n, head_dim);

        for (int i=0; i<n; i++) {
            for (int j=0; j<head_dim; j++) {
                int col = h * head_dim + j;

                Qh[i][j] = Q[i][col];
                Kh[i][j] = K[i][col];
                Vh[i][j] = V[i][col];
            }
        }

        Matrix out = attention(Qh, Kh, Vh);

        for (int i=0; i<n; i++) {
            for (int j=0; j<head_dim; j++) {
                int col = h * head_dim + j;
                result[i][col] = out[i][j];
            }
        }
    }

    return result;
}

// Linear Layer 구현
Matrix linear(const Matrix& X,
              const Matrix& W,
              const Vector& b) {
    
    if (X.cols() != W.rows()) throw runtime_error("Matrix size not match!! (X, W)");
    if (W.cols() != b.size()) throw runtime_error("Bias size not match!!");

    Matrix result = X * W;

    for (int i=0; i<result.rows(); i++) {
        for (int j=0; j<result.cols(); j++) {
            result[i][j] += b[j];
        }
    }

    return result;
}

// Relu 코드
Matrix relu(const Matrix& m) {
    Matrix result = m;

    for (int i=0; i<result.rows(); i++) {
        for (int j=0; j<result.cols(); j++) {
            if (result[i][j] < 0) {
                result[i][j] = 0;
            }
        }
    }

    return result;
}

// Feed Forward 함수
Matrix feedForward(const Matrix& X,
                   const Matrix& W1, const Vector& b1,
                   const Matrix& W2, const Vector& b2) {

    Matrix hidden = linear(X, W1, b1);

    hidden = relu(hidden);

    Matrix result = linear(hidden, W2, b2);

    return result;
}

// 
Matrix transformerBlock(const Matrix& X,
                        const Matrix& Wq, const Vector& bq,
                        const Matrix& Wk, const Vector& bk,
                        const Matrix& Wv, const Vector& bv,
                        const Matrix& W1, const Vector& b1,
                        const Matrix& W2, const Vector& b2,
                        int num_heads) {

    Matrix Q = linear(X, Wq, bq);
    Matrix K = linear(X, Wk, bk);
    Matrix V = linear(X, Wv, bv);

    // Multi-head Attention
    Matrix attn = multiHeadAttention(Q, K, V, num_heads);

    // Residual
    Matrix H = X + attn;
    H = layerNorm(H);

    // ff
    Matrix ff = feedForward(H, W1, b1, W2, b2);

    // Residual
    Matrix Y = H + ff;
    Y = layerNorm(Y);

    return Y;
}

Matrix layerNorm(const Matrix& X) {
    double gamma = 1, betta = 0;
    Matrix Y(X.rows(), X.cols());

    for (int i=0; i<X.rows(); i++) {
        double mean=0, std=0;

        for (int j=0; j<X.cols(); j++) mean += X[i][j];
        mean /= X.cols();

        for (int j=0; j<X.cols(); j++) std += pow(X[i][j] - mean, 2);
        std = sqrt(std / X.cols());

        for (int j=0; j<X.cols(); j++) {
            Y[i][j] = ((X[i][j] - mean) / (std + 1e-6)) * gamma + betta;
        }
    }

    return Y;
}

// 아래는 예제

void returnMatrixCout(const Matrix& A) {
    for (int i=0; i<A.rows(); i++) {
        cout << "|  ";
        for (int j=0; j<A.cols(); j++) {
            cout << A[i][j] << " ";
        }
        cout << " |\n";
    }
}

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

void program4() {
    Matrix A({{1,2,3},{4,5,6}});
    Matrix B = A.transpose();
    
    returnMatrixCout(B);
    return;
}

void program5() {

    Matrix Q({{1,0},{0,1}});
    Matrix K({{1,0},{0,1}});
    Matrix V({{10,0},{0,20}});

    Matrix out = attention(Q, K, V);

    returnMatrixCout(out);
    return;
}

void program6() {

    Matrix X({{1,2},{3,4}});
    Matrix W({{1,0},{0,1}});
    Vector b({1,1});

    Matrix Y = linear(X, W, b);

    returnMatrixCout(Y);
}

// 여기서부터는 메인

int main() {
    
    cout << "[START]==============\n\n";

    program6();

    while (true) {
        string tmp;
        cout << "\n================[END]\nType 'n' to end\n";
        getline(cin, tmp);
        if (tmp == "n") break;
    }
    return 0;
};
