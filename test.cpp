#include "matrix_recovery.hpp"
#include <random>

bool CheckResult(int N, std::vector<std::vector<int>> &v, std::vector<int> &result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (v[result[i]][j] != v[result[N + j]][i]) {
                return false;
            }
        }
    }
    return true;
}

std::mt19937 rnd(115150624); // random

TEST(UnitTest, SmallMatrix2x2) {
    int N = 2;
    std::vector<std::vector<int>> v(2*N);
    v[0] = {1, 2};
    v[1] = {3, 4};
    v[2] = {2, 4};
    v[3] = {1, 3};
    std::vector<int> result(2*N);
    bool has_result = false;
    NMatrixRecovery::FindMatrix(N, std::ref(v), std::ref(result), std::ref(has_result));
    ASSERT_TRUE(has_result);
    ASSERT_TRUE(CheckResult(N, std::ref(v), std::ref(result)));
}

TEST(UnitTest, SmallMatrix2x2NotFound) {
    int N = 2;
    std::vector<std::vector<int>> v(2*N);
    v[0] = {1, 2};
    v[1] = {5, 4};
    v[2] = {5, 4};
    v[3] = {1, 5};
    std::vector<int> result(2*N);
    bool has_result = false;
    NMatrixRecovery::FindMatrix(N, std::ref(v), std::ref(result), std::ref(has_result));
    ASSERT_FALSE(has_result);
}

TEST(UnitTest, BigMatrix10x10) {
    int N = 5;
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (i + j) * 3 - 10;
        }
    }
    std::vector<int> permutation(2*N);
    for (int i = 0; i < 2*N; i++) {
        permutation[i] = i;
    }
    std::random_shuffle(permutation.begin(), permutation.end());
    std::vector<std::vector<int>> v(2*N, std::vector<int>(N));
    for (int i = 0; i < 2*N; i++) {
        int pos = permutation[i] % N;
        for (int j = 0; j < N; j++) {
            if (permutation[i] >= N) {
                v[i][j] = matrix[j][pos]; 
            } else {
                v[i][j] = matrix[pos][j];
            }
        }
    }
    std::vector<int> result(2*N);
    bool has_result = false;
    NMatrixRecovery::FindMatrix(N, std::ref(v), std::ref(result), std::ref(has_result));
    ASSERT_TRUE(has_result);
    ASSERT_TRUE(CheckResult(N, std::ref(v), std::ref(result)));
}

TEST(StressTest, Matrix100x100) {
    int N = 100;
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rnd() % 256;
        }
    }
    std::vector<int> permutation(2*N);
    for (int i = 0; i < 2*N; i++) {
        permutation[i] = i;
    }
    std::random_shuffle(permutation.begin(), permutation.end());
    std::vector<std::vector<int>> v(2*N, std::vector<int>(N));
    for (int i = 0; i < 2*N; i++) {
        int pos = permutation[i] % N;
        for (int j = 0; j < N; j++) {
            if (permutation[i] >= N) {
                v[i][j] = matrix[j][pos]; 
            } else {
                v[i][j] = matrix[pos][j];
            }
        }
    }
    std::vector<int> result(2*N);
    bool has_result = false;
    NMatrixRecovery::FindMatrix(N, std::ref(v), std::ref(result), std::ref(has_result));
    ASSERT_TRUE(has_result);
    ASSERT_TRUE(CheckResult(N, std::ref(v), std::ref(result)));
}