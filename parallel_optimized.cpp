/*
 * g++ -O3 -ffast-math -fwrapv -std=c++11 main.cpp -fopenmp -DSIZE=1000
 * ./a.out
 */

#include <iostream>
#include <chrono>

#ifndef SIZE
#define SIZE 1000
#endif

using namespace std;
double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

int blockSize = 8;

double multiply_parallel_optimized();

int main(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
            C[i][j] = rand();
        }
    }
    double a = multiply_parallel_optimized();
    cout << a << endl;
}


double multiply_parallel_optimized() {

    auto t1 = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < SIZE; ++i) {
        for (int j = i; j < SIZE; ++j) {
            double temp = B[i][j];
            B[i][j] = B[j][i];
            B[j][i] = temp;
        }
    }

#pragma omp parallel for
    for (int is = 0; is < SIZE; is += blockSize) {
#pragma omp parallel for
        for (int is2 = 0; is2 < SIZE; is2 += blockSize) {
            for (int j = is2; j < is2 + blockSize && j < SIZE; ++j) {
                for (int i = is; i < is + blockSize && i < SIZE; ++i) {
                    double sum = 0;
                    for (int k = 0; k < SIZE; ++k)
                        sum += A[i][k] * B[j][k];
                    C[i][j] = sum;

                }
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}
