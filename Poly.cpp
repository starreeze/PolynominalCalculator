#include "Poly.h"
#include<algorithm>
#include<cmath>
#include<sstream>

//accuracy of Equation and zero
constexpr double EqA = 1e-9, ZeA = 1e-9;
constexpr unsigned EqT = 100;

bool Poly::is_0() const {
	return coe.empty() || !coe[0];
}

double Poly::operator()(double x) const {
	double r = 1.0;
	for (int i = 0; i < coe.size(); ++i)
		r += coe[i] * pow(x, i);
	return r;
}

pair<Poly, Poly> Poly::divide(const Poly& p) const {
	if (p.is_0())	throw string("Math error: divided by Zero.\n");
	deque<double> r, a(coe.rbegin(), coe.rend()), b(p.coe.rbegin(), p.coe.rend());
	while (a.size() >= b.size()) {
		double t = a.front() / b.front();
		for (int i = 1; i < b.size(); ++i)
			a[i] -= t * b[i];
		a.pop_front();
		r.push_front(t);
	}
	reverse(a.begin(), a.end());
	return { r,a };
}

Poly::Poly(const deque<double>& c) {
	coe = c;
	for (int i = coe.size() - 1; i >= 0 && abs(coe[i]) < ZeA; --i)
		coe.erase(coe.begin() + i);
}

Poly Poly::operator+(const Poly& p) const {
	deque<double> r;
	for (int i = 0; i < coe.size() || i < p.coe.size(); ++i) {
		double t = 0.0;
		if (i < coe.size())	t += coe[i];
		if (i < p.coe.size())	t += p.coe[i];
		r.push_back(t);
	}
	return r;
}

Poly Poly::operator-(const Poly& p) const {
	Poly t(p);
	for (auto& i : t.coe)	i = -i;
	return *this + t;
}

Poly Poly::operator*(const Poly& p) const {
	Poly r(0.0);
	for (int i = 0; i < coe.size(); ++i) {
		deque<double> t;
		t.assign(i, 0.0);
		for (int j = 0; j < p.coe.size(); ++j)
			t.push_back(p.coe[j] * coe[i]);
		r = r + t;
	}
	return r;
}

Poly Poly::operator/(const Poly& p) const {
	return divide(p).first;
}

Poly Poly::operator%(const Poly& p) const {
	return divide(p).second;
}

Poly Poly::d() const {
	if (coe.size() <= 1)	return 0.0;
	deque<double> r;
	for (int i = 1; i < coe.size(); ++i)
		r.push_back(i * coe[i]);
	return r;
}

Poly Poly::inv(char x) const {
	//𝑏_𝑘=∑_(𝑖=0)^(𝑘−1)〖𝑏_𝑖*(−𝑎_(𝑘−𝑖)/𝑎_0〗
	if (coe.empty() || abs(coe[0]) < ZeA)
		throw string("Math error: polynominal \'") + string{x} +"\' doesn't have an inverse element.\n";
	deque<double> r{ 1 / coe[0] };
	for (int k = 1; k < coe.size(); ++k) {
		double t = 0.0;
		for (int i = 0; i < k; ++i)
			t += r[i] * (-coe[k - i] / coe[0]);
		r.push_back(t);
	}
	return r;
}

double Poly::I(double a, double b) const {
	auto r = (*this);
	r.coe.push_front(0.0);
	for (int i = 1; i < r.coe.size(); ++i)
		r.coe[i] /= i;
	return r(b) - r(a);
}

double Poly::solve() const {
	double x = 1.1, x0 = 1e9;
	for (int time = 0; abs(x - x0) > EqA && time < EqT; ++time) {
		x0 = x; double t = d()(x);
		if (abs(t) < ZeA)	break;
		x -= operator()(x) / d()(x);
	}
	if (abs(operator()(x)) < ZeA)	return abs(x) < ZeA ? 0.0 : x;
	cout << "Input a number as x0: ";
	cin >> x; cin.get(); x0 = 1e9;
	for (int time = 0; abs(x - x0) > EqA && time < EqT; ++time) {
		x0 = x; double t = d()(x);
		if (abs(t) < ZeA)	break;
		x -= operator()(x) / d()(x);
	}
	if (abs(operator()(x)) > ZeA)
		throw string("Math error: cannot find a solution with given beginning value.\n");
	return abs(x) < ZeA ? 0.0 : x;
}

ostream& operator<<(ostream& o, const Poly& p) {
	if (p.coe.empty() || p.coe.size() == 1 && !p.coe[0]) {
		o << "0\n"; return o;
	}
	for (int i = p.coe.size() - 1; i >= 0; --i) {
		if (p.coe[i] > ZeA) {
			if (i < p.coe.size() - 1)	o << " + ";
			if (abs(p.coe[i] - 1) > ZeA || !i)	o << p.coe[i];
			if (i == 1)	o << 'x';
			else if (i > 1)	o << "x^" << i;
		}
		else if (p.coe[i] < -ZeA) {
			o << " - ";
			if (abs(p.coe[i] + 1) > ZeA || !i)	o << -p.coe[i];
			if (i == 1)	o << 'x';
			else if (i > 1)	o << "x^" << i;
		}
	}
	o << '\n';
	return o;
}
