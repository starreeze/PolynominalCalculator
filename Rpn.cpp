#include "Rpn.h"
#include"Poly.h"
#include<unordered_map>
#include<stack>
#include<algorithm>
#include<cassert>
#include<cstdlib>
#include <boost/algorithm/string.hpp>
extern unordered_map<string, Poly> mem;

bool isChar(char x) {
	return x >= 'A' && x <= 'Z' || x >= 'a' && x <= 'z';
}
bool isNum(char x) {
	return x >= '0' && x <= '9' || x == '.' || x == '-';
}

//the type of an operator
//priority: back > front > double
bool isDouble(char x) {
	return boost::is_any_of("+-*/%")(x);
}
bool isFrontSingle(char x) {
	return boost::is_any_of("$]")(x);
}
bool isBackSingle(char x) {
	return boost::is_any_of("!\'")(x);
}
void out(string opers, stack<string>& src, vector<string>& dst) {
	//move all the elements in opers from src to dst
	while (!src.empty() && find(opers.begin(), opers.end(), src.top()[0]) != opers.end()) {
		auto t = src.top(); src.pop();
		dst.push_back(t);
	}
}

vector<string> toRpn(string expr) {
	vector<string> r;
	stack<string> s;
	for (int i = 0; i < expr.size(); ++i) {
		if (isChar(expr[i])) {
			string t;
			for (; isChar(expr[i]); ++i)	t.push_back(expr[i]);
			r.push_back(t);
			--i;
		}
		else if (isNum(expr[i]) && expr[i] != '-') {
			string t;
			for (; isNum(expr[i]); ++i)	t.push_back(expr[i]);
			r.push_back(t);
			--i;
		}
		else if (isBackSingle(expr[i])) {
			out("!\'", s, r);
			s.push({ expr[i] });
		}
		else if (expr[i] == '$') {
			int j = i;
			if (expr[++i] != '[')	throw string("Syntax error: \'$\' is not followed by a range.\n");
			for (++i; i < expr.size() && isNum(expr[i]); ++i);
			if (i == expr.size() || expr[i] != ',')
				throw string("Syntax error: \'[\' is not followed by a pair of comma-seperated numbers.\n");
			for (++i; i < expr.size() && isNum(expr[i]); ++i);
			if (i == expr.size())	throw string("Syntax error: bracket \'[\' is not balanced.\n");
			if(expr[i] != ']')
				throw string("Syntax error: \'[\' is not followed by a pair of comma-seperated numbers.\n");
			out("!\'", s, r);
			s.push(string(expr.begin() + j, expr.begin() + i + 1));
		}
		else if (boost::is_any_of("*/%")(expr[i])) {
			out("!\'$", s, r);
			s.push({ expr[i] });
		}
		else if (expr[i] == '+' || expr[i] == '-') {
			out("!\'$*/%", s, r);
			s.push({ expr[i] });
		}
		else if (expr[i] == '(')	s.push("(");
		else if (expr[i] == ')') {
			while (!s.empty() && s.top()[0] != '(') {
				auto t = s.top(); s.pop();
				r.push_back(t);
			}
			if (s.empty())    throw string("Syntax error: bracket \')\' is not balanced.\n");
			s.pop();
		}
		else    throw string("Syntax error: unrecognized symbol \'") + string({ expr[i] }) + "\'.\n";
	}
	while (!s.empty()) {
		auto t = s.top(); s.pop();
		if (t[0] == '(')    throw string("Syntax error: bracket \'(\' is not balanced.\n");
		r.push_back(t);
	}
	return r;
}

Poly eval(const vector<string>& rpn) {
	stack<Poly> s;
	for (int i = 0; i < rpn.size(); ++i) {
		if (isChar(rpn[i][0])) {
			auto t = mem.find(rpn[i]);
			if (t != mem.end())	s.push(t->second);
			else    throw string("Symbolic error: undefined identifier \'") + rpn[i] + "\'.\n";
		}
		else if (isNum(rpn[i][0]) && rpn[i][0] != '-')	s.push(atof(rpn[i].c_str()));
		else switch (rpn[i][0]) {
		case '!': s.top() = s.top().d(); break;
		case '\'': s.top() = s.top().inv(rpn[i - 1][0]); break;
		case '$': s.top() = s.top().I(atof(rpn[i].substr(2).c_str()),
			atof(rpn[i].substr(rpn[i].find(',') + 1).c_str()));
			break;
		case '*': {
			auto t = s.top(); s.pop();
			s.top() = s.top() * t;
			break;
		}case '/': {
			auto t = s.top(); s.pop();
			s.top() = s.top() / t;
			break;
		}case '%': {
			auto t = s.top(); s.pop();
			s.top() = s.top() % t;
			break;
		}
		case '+': {
			auto t = s.top(); s.pop();
			s.top() = s.top() + t;
			break;
		}
		case '-': {
			auto t = s.top(); s.pop();
			s.top() = s.top() - t;
			break;
		}
		}
	}
	assert(s.size() == 1);
	return s.top();
}
