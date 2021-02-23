#include<iostream>
#include<string>
#include<vector>

using namespace std;
string symbol[]= { "-","+","*","/","%" };
vector<string>tokens;
vector<string>com;
string current_token;
int flag = 0;
int Exp2();
int Exp1();
void Expression(string s) {
	cout << "expression: (" << s << ")" << endl;
}
void SyntaxError(int k) {
	flag = 1;
	cout << "Raise Error:";
	switch (k)
	{
	case 1: {
		cout << "First text must be digit or '('[#1]" << endl;
		break;
	}
	case 2: {
		cout << "The front of digit must be op or '('[#2]" << endl;
		break;
	}
	case 3: {
		cout << "The front of op must be digit or ')'[#3]" << endl;
		break;
	}
	case 4: {
		cout << "The front of '(' must be op or '('[#4]" << endl;
		break;
	}
	case 5: {
		cout << "The front of ')' must be digit or ')'[#5]" << endl;
		break;
	}
	case 6: {
		cout << "Expression contains undefined symbol![#6]" << endl;
		break;
	}
	case 7: {
		cout << "Missing the ')'[#7]" << endl;
		break;
	}
	case 8: {
		cout << "Expression Error![#8]" << endl;
		break;
	}
	case 9: {
		cout << "Can't devide the zero![#9]" << endl;
		break;
	}
	default:
		break;
	}
}
bool is_number(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] >= '0' && s[i] <= '9')
			continue;
		else
			return false;
	}
	return true;
}
int Found(string s) {
	for (int i = 0; i < 5; i++) {
		if (s == symbol[i])
			return true;
	}
	return false;
}
string Next_token() {
	if (!tokens.empty()) {
		tokens.erase(tokens.begin());
		if (!tokens.empty())
			return tokens.front();
		else
			return"";
	}
	return "";
}
void Token(string s) {//将表达式输出为token
	string temp = "";
	for (int i = 0; i < s.size(); i++) {
		temp = "";
		if (s[i] >= '0' && s[i] <= '9') {
			while (s[i] >= '0' && s[i] <= '9') {
				temp += s[i];
				i++;
			}
			tokens.push_back(temp);
			temp = "";
			i--;
		}

		else {
			temp.push_back(s[i]);
			tokens.push_back(temp);
		}
	}
	cout << "Token:[";
	int i = 0;
	for (i; i < tokens.size()-1; i++) {
		cout << tokens[i] << ",";
	}
	cout<<tokens[i] << "]" << endl;
	return;
}
void Check_Token() {//校验token的合法性,优化
	int i = 0;
	for (i; i < tokens.size(); i++) {
		if (i == 0) {
			if (!(tokens[i] == "(" || is_number(tokens[i])))
				SyntaxError(1);//第一个字符不是数字或者(
				continue;
		}
		if (is_number(tokens[i])) {
			if (!(Found(tokens[i - 1]) || (tokens[i - 1] == "(")))
				SyntaxError(2);//数字前面不是(或者运算符
				continue;
		}
		if (Found(tokens[i])) {
			if (!(is_number(tokens[i - 1]) || tokens[i - 1] == ")"))
				SyntaxError(3);//运算符前不是数字或者)
				continue;
		}
		if (tokens[i] == "(") {
			if (!(Found(tokens[i - 1]) || tokens[i - 1] == "("))
				SyntaxError(4);//(前不是运算符或者数字
				continue;
		}
		if (tokens[i] == ")") {
			if (!(is_number(tokens[i - 1]) || tokens[i - 1] == ")"))
				SyntaxError(5);//)前不是数字或者)
				continue;
		}
		else
			SyntaxError(6);//包含其余字符
	}
	if(!flag)
	cout << "token pass the checking(I)..." << endl;
}
int Factor() {
	if (is_number(current_token)&&current_token!="") {//数字
		int r = stoi(current_token);
		current_token = Next_token();
		return r;
	}
	if (current_token == "(") {
		current_token = Next_token();
		int r = Exp1();
		if (current_token != ")") {
			SyntaxError(7);
		}
		current_token = Next_token();
		return r;
	}
	else
		SyntaxError(8);
}
int Exp1() {
	int t1 = Exp2();
	string oper = current_token;
	while (oper == "+" || oper == "-") {
		current_token = Next_token();
		int t2 = Exp2();
		if (oper == "+") {
			t1 += t2;
		}
		else {
			t1 -= t2;
		}
		oper = current_token;
	}
	return t1;
}
int Exp2() {
	int t1 = Factor();
	string oper = current_token;
	while (oper == "*" || oper == "/" || oper == "%") {
		current_token = Next_token();
		int t2 = Factor();
		if (oper == "*") {
			t1 *= t2;
		}
		if (oper == "/") {
			if (t2 == 0) {
				SyntaxError(9);
				t2 = 1;
			}
			t1 /= t2;
		}
		if (oper == "%") {
			if (t2 == 0) {
				SyntaxError(9);
				t2 = 1;
			}
			t1 %= t2;
		}
		oper = current_token;
	}
	return t1;
}
void Inicial() {
	com.clear();
	com.push_back("exit");
	com.push_back("clc");
	com.push_back("help");
	com.push_back("test");
}
void Info(int k) {
	switch (k)
	{
	case 0: {
		cout << "Do you mean 'exit'?" << endl;
		return;
	}
	case 1: {
		cout << "Do you mean 'clc'" << endl;
		return;
	}
	case 2: {
		cout << "Do you mean 'help'" << endl;
		return;
	}
	case 3: {
		cout << "Do you mean 'test'" << endl;
		return;
	}
	default:
		break;
	}
}
void Calc(string expr) {
	if (expr == "")return;
	int k = -1;
	for (int i = 0; i < com.size(); i++) {
		if (com[i].find(expr) != string::npos)
			if (!(com[i] == expr)) {
				Info(i);
				return;
			}
			else k = i;
	}
		switch (k)
		{
		case 0: {
			cout << "GoodBye!" << endl;
			exit(1);
			getchar();
		}
		case 1: {
			system("cls");
			cout << "Calc Program @Copyright 2021 by DJH" << endl;
			cout << "if you have question, pls contact me at djh113@126.com" << endl << endl;
			break;
		}
		case 2: {
			cout << "welcome to the Calc,All the Commands this system has are following:" << endl << endl;
			cout << "[exit] ## exit from the system" << endl;
			cout << "[help] ## give you some info about this system" << endl;
			cout << "[clc] ## clear all,your result will be cleared" << endl;
			cout << "[test] ## give you some example" << endl;
			cout << "[express] ## input like '(3+2-6)*9%4',and system will output the result" << endl;
			cout << "to be continue..." << endl << endl;
			break;
		}
		case 3: {
			Expression("(3+2-6)*9%4+997");
			Token("(3+2-6)*9%4+997");
			Check_Token();
			current_token = tokens[0];
			int r = Exp1();
			cout <<"res:"<< r << endl;
			tokens.clear();
			Expression("(((5))+4/((3))-7*(6-5/(3-(6-4))))");
			Token("(((5))+4/((3))-7*(6-5/(3-(6-4))))");
			Check_Token();
			current_token = tokens[0];
			r = Exp1();
			cout << "res:" << r << endl;
			break;
		}
		default: {
			Expression(expr);
			Token(expr);
			Check_Token();
			current_token = tokens[0];
			int r = Exp1();
			if (!flag) {
				cout << "res:" << r << endl;
			}
			break;
		}
		}
}
int main() {
	/*初始化程序*/
	system("title Calc System @Copyright 2021 by DJH");
	system("mode con cols=80 lines=25");
	system("cls");
	cout << "Calc Program @Copyright 2021 by DJH" << endl;
	cout << "if you have question, pls contact me at djh113@126.com" << endl << endl;
	Inicial();
	/*进入循环读取表达式部分*/
	while (true) {
		flag = 0;
		cout << "Calc#>>>";
		string input;
		getline(cin, input);
		tokens.clear();
		Calc(input);//读取用户输入的内容
	}
	return 0;
}