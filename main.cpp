#include <bits/stdc++.h>
using namespace std;

const size_t MAX_THREAD_COUNT = 8;

int main() {
    int N;
    std::cin >> N;
    std::vector<std::vector<int>> v(2*N, std::vector<int>(N));
    for (int i = 0; i < 2*N; i++) {
        for (int j = 0; j < N; j++) {
            std::cin >> v[i][j];
        }
    }

    vector<int> permutation(2*N);
    vector<int> res(2*N);
    shared_ptr<vector<int>> res_ptr(&res);
    for (int i = 0; i < 2 * N; i++) {
        permutation[i] = i;
    }

    auto func = [res_ptr, N](const vector<int> permutation,
                             const vector<vector<int>> &v,
                             shared_ptr<std::atomic_bool> f_ptr) {
        if ((*f_ptr).load()) {
            return;
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (v[permutation[i]][j] != v[N + permutation[j]][i]) {
                    return;
                }
            }
        }
        bool result = false;
        if ((*f_ptr).compare_exchange_weak(result, true)) {
            for (int i = 0; i < permutation.size(); i++) {
                (*res_ptr)[i] = permutation[i];
            }
        }
    };

    vector<std::thread> threads;
    int k = 0;
    std::atomic_bool is_find{false};
    shared_ptr<std::atomic_bool> is_find_ptr(&is_find);
    do {
        if (threads.size() == MAX_THREAD_COUNT) {
            for (std::thread &th : threads) {
                th.join();
            }
            threads.clear();
        }
        threads.push_back(std::thread(func, permutation, v, is_find_ptr));
        k++;
    } while (next_permutation(permutation.begin(), permutation.end()) && !(*is_find_ptr).load());
    if (threads.size()) {
        for (std::thread &th : threads) {
            th.join();
        }
        threads.clear();
    }

    for (int j = 0; j < 2*N; j++) {
        cout << (*res_ptr)[j];
    }
    cout << endl;

    return 0;
}