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


double transpose4();
double transpose5();

int blockSize=4;

int main(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
            C[i][j] = rand();
        }
    }
    double a;
#pragma omp parallel

    a = transpose4();
    cout << a  << endl;

}


double transpose4() {

    auto t1 = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int is = 0; is < SIZE; is += blockSize) {
        for (int i = is; i < is + blockSize && i < SIZE; ++i) {
#pragma omp parallel for
            for (int is2 = 0; is2 < i; is2 += blockSize) {
                for (int j = is2; j < is2 + blockSize && j <  i; ++j) {
                    double temp=B[i][j];
                    B[i][j]=B[j][i];
                    B[j][i] =temp;
                    C[i][j]=0;
                    C[j][i]=0;
                }
            }
        }
    }



#pragma omp parallel for
    for (int is = 0; is < SIZE; is += blockSize) {
#pragma omp parallel for
        for (int is2 = 0; is2 < SIZE; is2 += blockSize) {
            for (int j = is2; j < is2 + blockSize && j < SIZE; ++j) {
                for (int i = is; i < is + blockSize && i < SIZE; ++i) {
                    double sum=0;
                    //#pragma omp parallel for reduction(+:sum)
                    for (int k = 0; k < SIZE; ++k)
                        sum += A[i][k] *B[j][k];

                    C[i][j]=sum;

                }
            }
        }
    }



    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}

double transpose5() {

    auto t1 = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int is = 0; is < SIZE; is += blockSize) {
#pragma omp parallel for
        for (int is2 = 0; is2 < is+ blockSize; is2 += blockSize) {

            for (int i = is; i < is + blockSize && i < SIZE; ++i) {

                for (int j = is2; j < is2 + blockSize && j <  i; ++j) {
                    double temp=B[i][j];
                    B[i][j]=B[j][i];
                    B[j][i] =temp;
                    C[i][j]=0;
                    C[j][i]=0;
                }
            }
        }
    }



#pragma omp parallel for
    for (int is = 0; is < SIZE; is += blockSize) {
#pragma omp parallel for
        for (int is2 = 0; is2 < SIZE; is2 += blockSize) {

            for (int j = is2; j < is2 + blockSize && j < SIZE; ++j) {

                for (int i = is; i < is + blockSize && i < SIZE; ++i) {
                    double sum=0;
//                    #pragma omp parallel for reduction(+:sum)
                    for (int k = 0; k < SIZE; ++k)
                        sum += A[i][k] *B[j][k];

                    C[i][j]=sum;

                }
            }
        }
    }


    auto t2 = std::chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}