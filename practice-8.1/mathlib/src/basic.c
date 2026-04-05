#include <math.h>
#include "mathlib.h"

double ml_sqrt(double x) {
    double guess = x / 2.0;
    double prev_guess;
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
    } while (fabs(guess - prev_guess) > 1e-12);
    return guess;
}

long long ml_factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}