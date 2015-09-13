/*
 * g++ -O3 -ffast-math -fwrapv -ftree-loop-optimize -ftree-loop-vectorize
 *     -std=c++11 main.cpp -fopenmp -DSIZE=1000
 * ./a.out
 */



#include <iostream>
#include <vector>
#include <chrono>


#ifndef SIZE
#define SIZE 1000
#endif

using namespace std;
double A[SIZE][SIZE];
double B[SIZE][SIZE];
double C[SIZE][SIZE];

double serial();



int main(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
            C[i][j] = rand();
        }
    }


    double a;
    a = serial();
    cout << a << endl;

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

}

