#ifndef SUMMING_H
#define SUMMING_H

#include <vector>
using std::vector;

template <typename FPtype>
FPtype Kahan_sum(const vector<FPtype> &vals)
{
    FPtype sum = 0;
    FPtype c = 0;
    FPtype y, t;
    for (auto &x : vals)
    {
        y = x - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

/*
In this algorithm:

sum is the running total.
c is the compensation variable, which accumulates small errors in the calculation.
y is the current input number minus the previous compensation.
t is the temporary sum of sum and y.
The Kahan summation algorithm significantly reduces the numerical error compared to the straightforward approach of simply summing the numbers in sequence12. The worst-case error of the Kahan summation algorithm is effectively independent of the number of values being summed, so a large number of values can be summed with an error that only depends on the floating-point precision of the result.*/

template <typename T>
T trivial_sum(const vector<T> &vals)
{
    T sum = 0;
    for (auto &x : vals)
    {
        sum = sum + x;
    }
    return sum;
}

#endif