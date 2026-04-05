#ifndef MATHLIB_H
#define MATHLIB_H

#define MATHLIB_PI 3.14159265358979323846
#define MATHLIB_E  2.71828182845904523536

double ml_abs(double x);
double ml_pow(double base, int exp);
double ml_sqrt(double x);
long long ml_factorial(int n);
long long ml_gcd(long long a, long long b);
double ml_sin(double x);
double ml_cos(double x);

#endif