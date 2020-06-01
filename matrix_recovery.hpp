#include <vector>
#include <atomic>
#include <thread>
#include <algorithm>
#include <iostream>

namespace NMatrixRecovery {
    namespace {
        const size_t kMaxThreadCount = 8;

        void CheckPermutation(const std::vector<int> permutation, 
                  std::vector<std::vector<int>> &v,
                  std::vector<int> &result,
                  std::atomic_bool &found,
                  int N) {
            if (found.load()) {
                return;
            }
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (v[permutation[i]][j] != v[permutation[N + j]][i]) {
                        return;
                    }
                }
            }
            if (!found.exchange(true)) {
                for (int i = 0; i < permutation.size(); i++) {
                    result[i] = permutation[i];
                }
            }
        }
    }

    void FindMatrix(int N, std::vector<std::vector<int>> &v,
               std::vector<int> &result, bool &has_result) {

        std::vector<int> permutation(2*N);
        for (int i = 0; i < 2 * N; i++) {
            permutation[i] = i;
        }

        std::vector<std::thread> threads;
        std::atomic_bool found{false};

        do {
            if (threads.size() == kMaxThreadCount) {
                for (std::thread &th : threads) {
                    th.join();
                }
                threads.clear();
            }
            threads.emplace_back(std::thread(CheckPermutation, permutation, std::ref(v), std::ref(result), std::ref(found), N));
        } while (next_permutation(permutation.begin(), permutation.end()) && !(found).load());

        if (threads.size()) {
            for (std::thread &th : threads) {
                th.join();
            }
            threads.clear();
        }

        if (found.load()) {
            has_result = true;
        } else {
            has_result = false;
        }
    }

}; //namespace NMatrixRecovery