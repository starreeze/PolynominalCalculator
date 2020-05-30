#pragma once
#include<deque>
#include<iostream>
using namespace std;
class Poly
{
	deque<double> coe;//coefficient, reversed
	bool is_0() const;
	double operator()(double x) const;
	pair<Poly, Poly> divide(const Poly& p) const;
public:
	Poly() {}
	Poly(double d) :coe({ d }) {}
	Poly(const deque<double>& c);
	Poly operator+(const Poly& p) const;
	Poly operator-(const Poly& p) const;
	Poly operator*(const Poly& p) const;
	Poly operator/(const Poly& p) const;
	Poly operator%(const Poly& p) const;
	Poly d() const;
	Poly inv(char x) const;
	double I(double a, double b) const;
	double solve() const;
	friend ostream& operator<<(ostream& o, const Poly& p);
};