#pragma once
#include<vector>
#include<string>
using namespace std;
class Poly;

bool isChar(char x);
bool isNum(char x);
bool isDouble(char x);
bool isFrontSingle(char x);
bool isBackSingle(char x);
vector<string> toRpn(string expr);
Poly eval(const vector<string>& rpn);