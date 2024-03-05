#include "matrix_maths.h"
#include <random>
#include <vector>
#include <iostream>
#include <chrono>
#include <functional>

using std::vector;
typedef std::chrono::high_resolution_clock myClock;

class Timer
{
public:
    std::chrono::time_point<myClock> time_start;
    std::chrono::time_point<myClock> time_end;
    double t_seconds()
    {
        return (time_end - time_start).count() / 1e9;
    }
    void start()
    {
        time_start = std::chrono::system_clock::now();
    }
    void end()
    {
        time_end = std::chrono::system_clock::now();
    }
};

void printMatrix2D(double **A, int N)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            std::cout << A[i][j] << " ";
            // std::cout << *(A);
        }
        std::cout << "\n";
    }
}

void cleanUp(double **A, int N)
{
    for (int i = 0; i < N; i++)
    {
        delete[] A[i];
    }
    delete[] A;
}
int main()
{
    std::mt19937_64 rng{2};
    std::uniform_real_distribution<double> dist(0, 1);
    auto generator = std::bind(dist, rng);
    const int N = 10;

    // Some options for matrix representations!
    // https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
    // double *X = new double[N * N];
    // vector<double> X(N*N);
    // double **X = new double X[N];
    // vector<vector<double>> X(N);

    // initialize
    // https://stackoverflow.com/questions/16001803/pointer-to-pointer-dynamic-two-dimensional-array
    double **X = new double *[N]; // this allocated in the heap -> created dynamic during runtime
    // double X[N][N]; // this allocated in the stack (limited memory) -> created in COMPILE time
    double *Y = new double[N * N];

    double **ans = new double *[N];

    for (size_t i = 0; i < N; i++)
    {
        X[i] = new double[N]; // HAVE to allocate per array in the pointer from L67
        ans[i] = new double[N];

        for (size_t j = 0; j < N; j++)
        {
            double value = generator();
            X[i][j] = value;
            // Y[j + N * i] = value;
            *(Y + i * N + j) = value;
        }
    }

    // https://stackoverflow.com/questions/8767166/passing-a-2d-array-to-a-c-function
    printMatrix2D(X, N);

    Timer timer;
    // row_by_rowMajor
    timer.start();
    rowMajor_by_rowMajor<double **>(X, X, ans, N);
    timer.end();
    std::cout << "Row a by row b multiplication: " << timer.t_seconds() << std::endl;

    // row_by_colMajor
    timer.start();
    rowMajor_by_colMajor<double **>(X, X, ans, N);
    timer.end();
    std::cout << "Row a by col b multiplication: " << timer.t_seconds() << std::endl;

    // row_by_rowMajor
    timer.start();
    trivial_transpose<double **>(X, ans, N);
    timer.end();
    std::cout << "Trivial transpose: " << timer.t_seconds() << std::endl;

    cleanUp(X, N);
    cleanUp(ans, N);
    return 0;
}