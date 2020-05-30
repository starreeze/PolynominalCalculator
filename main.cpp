#include<iostream>
#include<unordered_map>
#include <boost/algorithm/string.hpp>
#include"Poly.h"
#include"Rpn.h"

unordered_map<string, Poly> mem;

string getCmd() {
	char buf[100]; cin.getline(buf, 100);
	string r(buf);
	//check whether the operators are legal
	for (int i = 0; i < r.size(); ++i) {
		if (isDouble(r[i])) {
			if (!i || i == r.size() - 1 || !isChar(r[i - 1]) && !isNum(r[i - 1]) &&
				!boost::is_any_of(" ),[")(r[i - 1]) && !isBackSingle(r[i - 1])
				|| !isChar(r[i + 1]) && !isNum(r[i + 1]) && r[i + 1] != '(' && r[i + 1] != '$')
				throw string("Syntax error: illegal usage of operator \'") + string({ r[i] }) + "\'.\n";
		}
		else if (isBackSingle(r[i])) {
			if (!i || r[i - 1] != ')' && !isBackSingle(r[i - 1]) && !isChar(r[i - 1]) && !isNum(r[i - 1])
				|| i != r.size() - 1 && !isDouble(r[i + 1]) && r[i + 1] != ')' && !isBackSingle(r[i + 1]))
				throw string("Syntax error: illegal usage of operator \'") + string({ r[i] }) + "\'.\n";
		}
		else if (r[i] == '$') {
			int j = i + 1; for (; j < r.size() && r[j] != ']'; ++j);
			if (j >= r.size() - 1 || !isChar(r[j + 1]) && !isNum(r[j + 1]) && r[j + 1] != '(' && r[j + 1] != '$'
				|| i && !isDouble(r[i - 1]) && r[i - 1] != '(' && r[i - 1] != ']')
				throw string("Syntax error: illegal usage of operator \'$\'.\n");
		}
		else if (r[i] == '(' && r[i] != r.size() - 1 && r[i + 1] == ')')
			throw string("Syntax error: empty brackets \'()\'.\n");
		else if (isNum(r[i]) && r[i] != '-' && (i && isChar(r[i - 1]) || i < r.size() - 1 && isChar(r[i + 1]))) {
			int j = i, k = i;
			for (; j >= 0 && (isNum(r[j]) && r[j] != '-' || isChar(r[j])); --j);
			for (; k >= 0 && (isNum(r[k]) && r[k] != '-' || isChar(r[k])); ++k);
			throw string("Symbolic error: mixed number and identifier \'") + r.substr(j + 1, k) + "\'.\n";
		}
	}
	return r;
}

void processCmd(string s) {
	if (s.empty())	throw string();
	if (s == "exit")	exit(0);
	if (s == "show") {
		for (auto t : mem)
			cout << t.first << " = " << t.second;
		return;
	}
    auto pos = find(s.begin(), s.end(), '=');
	if (pos == s.end())	cout << eval(toRpn(s));
	else if (isChar(s[0])) { //assign a new value
		if (find_if(s.begin(), pos, [](char x) {return !isChar(x); }) != pos)
			throw string("Symbolic error: identifier of a polynominal can only be characters.\n");
		if (find_if(pos + 1, s.end(), [](char x) {return isChar(x); }) == s.end()) {
			deque<double> coe; //at reverse order
			vector<string> buf;
			string t = s.substr(pos - s.begin() + 1);
			boost::split(buf, t, boost::is_any_of(" ,"));
			for (auto t : buf)	coe.push_front(atof(t.c_str()));
			mem[s.substr(0, pos - s.begin())] = coe;
		}
		else    mem[s.substr(0, pos - s.begin())] = eval(toRpn(s.substr(pos - s.begin() + 1)));
	}
	else if (s[0] == '0') {
		if (pos - s.begin() != 1)
			throw string("Syntax error: unrecognized identifier \'") + s.substr(0, pos - s.begin()) + "\'.\n";
		cout << "x = " << eval(toRpn(s.substr(pos - s.begin() + 1))).solve() << '\n';
	}
	else    throw string("Syntax error: unrecognized identifier \'") + s.substr(0, pos - s.begin()) + "\'.\n";
}

int main() {
	while (true) {
		cout << ">>> ";
		try { processCmd(getCmd()); }
		catch (const string& s) { cout << s; }
	}
	return 0;
}
