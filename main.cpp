/*
 * g++ -O3 -ffast-math -fwrapv -ftree-loop-optimize -ftree-loop-vectorize
 *     -std=c++11 main.cpp -fopenmp -DSIZE=1000
 * ./a.out
 */



#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

#ifndef SIZE
#define SIZE 1000
#endif

using namespace std;
double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

double serial();
double naive();
double transpose();
double transpose2();

int threads;

int main(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
            C[i][j] = rand();
        }
    }

#pragma omp parallel
    threads = omp_get_num_threads();


    double a, b, c, d;
    a = serial();
    cout << a << endl;
    b = naive();
    cout << b << endl;
    c = transpose();
    cout << c << endl;
    d = transpose2();
    cout << d << endl;
    cout << a / c << endl;
    cout << a / d << endl;
}

double serial() {

    auto t1 = std::chrono::high_resolution_clock::now();
    int i, j;
    {
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                C[i][j] = 0;
                for (int k = 0; k < SIZE; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    //cout << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << endl;
}

double naive() {

    auto t1 = std::chrono::high_resolution_clock::now();
    int i, j;
    {
#pragma omp parallel for
        for (i = 0; i < SIZE; i++) {
#pragma omp parallel for
            for (j = 0; j < SIZE; j++) {
                C[i][j] = 0;
                for (int k = 0; k < SIZE; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    //cout << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << endl;
}

double transpose() {

    auto t1 = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        double temp;
        for (int j = i + 1; j < SIZE; j++) {
            temp = B[j][i];
            B[i][j] = B[j][i];
            B[j][i] = temp;
        }
    }
#pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * C[j][k];
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    //cout << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << endl;
}

double transpose2() {

    auto t1 = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int is = 0; is < SIZE; is += threads) {
#pragma omp parallel for
        for (int j = 0; j < SIZE; ++j) {
            double temp;
            for (int i = is; i < is + threads && i < SIZE; ++i) {
                temp = B[j][i];
                B[i][j] = B[j][i];
                B[j][i] = temp;
            }
        }
    }

#pragma omp parallel for
    for (int is = 0; is < SIZE; is += threads) {
#pragma omp parallel for
        for (int j = 0; j < SIZE; ++j) {
            for (int i = is; i < is + threads && i < SIZE; ++i) {
                for (int k = 0; k < SIZE; ++k)
                    C[i][j] += A[i][k] * B[j][k];
            }
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    //cout << chrono::duration_cast<chrono::microseconds>(t2-t1).count() << endl;
}