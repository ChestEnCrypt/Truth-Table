#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <unordered_map>

using namespace std;

string fvar;
unordered_map<string, function<bool()>> ffor;

function<bool()> ffun(string task);

pair<string, size_t> bracket(string task) {
	string t = "";
	size_t f = task.find('(');
	if (f == string::npos) return make_pair(task, 0);
	for (size_t i = f + 1, b = 1; i < task.size(); i++) {
		if (task[i] == '(') b++;
		else if (task[i] == ')') {
			if (--b == 0) {
				t = task.substr(f + 1, i - f - 1);
				return make_pair(t, i+1);
			};
		}
	}
	return make_pair(t, 0);
}

tuple<string, char, string> deleg(string task) {
	string first = "";
	string second = "";
	char oper = ' ';

	if (fvar.find(task[0]) != string::npos) {
		first = task.substr(0, 1);
		if (task.size() > 1) oper = task[1];
		else return make_tuple(first, oper, second);
		if (fvar.find(task[2]) != string::npos) second = task.substr(2, 1);
		else second = bracket(task.substr(2)).first;
		return make_tuple(first, oper, second);
	}
	auto b = bracket(task);
	first = b.first;
	if (task.size() < b.second + 1) return make_tuple(first, oper, second);
	oper = task[b.second];
	if (fvar.find(task[b.second + 1]) != string::npos) second = task.substr(b.second + 1);
	else second = bracket(task.substr(b.second + 1)).first;

	return make_tuple(first, oper, second);
}

function<bool()> ffun(string task) {

	if (ffor.find(task) != ffor.end()) return ffor[task];
	else if (task[0] == '!') {
		auto [f, o, s] = deleg(task.substr(1));
		ffor[f] = ffun(f);
		return [f = &ffor, t = f]() {return !(*f)[t]();};
	}
	else {
		auto [fs, op, sc] = deleg(task);

		if (ffor.find(fs) == ffor.end()) ffor[fs] = ffun(fs);
		if (sc == "") return ffor[fs];
		if (ffor.find(sc) == ffor.end()) ffor[sc] = ffun(sc);

		if (op == '+') {
			ffor[task] = [f = &ffor[fs], s = &ffor[sc]]() {return (*f)() || (*s)();};
		}
		else if (op == '*') {
			ffor[task] = [f = &ffor[fs], s = &ffor[sc]]() {return (*f)() && (*s)();};
		}
		else if (op == '>') {
			ffor[task] = [f = &ffor[fs], s = &ffor[sc]]() {return !(*f)() || (*s)();};
		}
	}

	return ffor[task];
}

int main() {
	cout << "Help for you" << endl;
	cout << "*, +, >	- operators" << endl;
	cout << "x, y, z	- variables" << endl;
	cout << "(x * y)	- formula" << endl;
	cout << "(!a)	- not a" << endl;
	cout << "a*b	- a and b" << endl;
	cout << "a+b	- a or b" << endl;
	cout << "a>b	- a -> b (a imp b)" << endl;

	string _task;
	getline(cin, _task);

	int nvar = 0;
	string task = "";

	for (char t : _task) {
		if (t == ' ') continue;
		if (('A' <= t && t <= 'z') && !(fvar.find(t) != string::npos)) {
			fvar += t;
			ffor[string(1, t)] = []() {return 0;};
			nvar++;
		}
		task += t;
	}

	ffor[task] = ffun(task);

	vector<string> vars;
	for (auto kv : ffor) vars.push_back(kv.first);
	sort(vars.begin(), vars.end(), [](const string& a, const string& b) {
		return a.size() < b.size();
	});

	for (auto& kv : vars) {
		cout << kv << '\t';
	}
	cout << endl;

	for (int i = 0; i < pow(2, nvar); i++) {
		int j = 0;
		for (auto kv : fvar) {
			ffor[string(1, kv)] = [b = (i >> j) & 1]() {return b;};
			j++;
		}

		for (auto kv : vars) {
			auto& val = ffor[kv];
			if (val) {
				cout << val();
				for (int k = 0; k < kv.size(); k += 8) {
					cout << '\t';
				}
			}
		}
		cout << endl;
	}

	return 0;
}
