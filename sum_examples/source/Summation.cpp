#include "Summing.h"
#include <iostream>
#include <vector> 
#include <chrono>
#include <random>
#include <functional>

using std::vector;

typedef std::chrono::high_resolution_clock myClock;

double t_seconds(std::chrono::time_point<myClock> t1, std::chrono::time_point<myClock> t2)
{
    return (t2 - t1).count() / 1e9;
}

/*
How to run with -ffast-math:
g++ -o ./build/bin/Sum source/Summation.cpp -Iinclude -o1 -ffast-math
*/
int main()
{
    std::random_device x{};
    std::mt19937_64 rng{x()};
    // std::mt19937_64 rng{2};
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    auto generator = std::bind(dist, rng);

    std::vector<float> number;
    int N = 1e6;
    for (size_t i = 0; i < N; i++)
    {
        number.push_back(generator());
    }
    // for (auto num: number)
    // {
    //     std::cout << num << " ";
    // }

    std::chrono::time_point<myClock> t_start = std::chrono::system_clock::now();
    float ans = Kahan_sum(number);
    std::chrono::time_point<myClock> t_end = std::chrono::system_clock::now();
    std::cout << "Result Kahan: " << ans << "\nTime taken: " << t_seconds(t_start, t_end) << std::endl;
    

    t_start = std::chrono::system_clock::now();
    float ans_trivial = trivial_sum(number);
    t_end = std::chrono::system_clock::now();
    std::cout << "Result Trivial: " << ans_trivial << "\nTime taken: " << t_seconds(t_start, t_end) << std::endl;
    return 0;
}

/*
g++ -o ./build/bin/Sum source/Summation.cpp -Iinclude -o1 -ffast-math
Result Kahan: 500110
Time taken: 0.00259207
Result Trivial: 500110
Time taken: 0.00261956

The Kahan result is the same with Trivial result because it ignores the smallest floating point
ffast-math: The machine code don't take the account c = (t - sum) - y; (?) like ignore some lines
t = sum + y;
c = (t - sum) - y; >>>> the machine will read both this machine code is the same, therefore c = 0;

MEANWHILE
g++ -o ./build/bin/Sum source/Summation.cpp -Iinclude
Result Kahan: 500119
Time taken: 0.00255323
Result Trivial: 500110
Time taken: 0.00211611

The Kahan result is more precise because it takes into account the smallest floating point !
*/