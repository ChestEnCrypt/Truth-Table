#include <iostream>
#include <cmath>
#include <tuple>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;

unordered_map<char, bool> fvar;
unordered_map<string, function<bool()>> ffor;

function<bool()> ffun(string task);

tuple<string, char, string> deleg(string task) {
	string first = "";
	string second = "";
	char oper = ' ';

	int bracked = 0;
	string b_content = "";
	for (int i = 0; i < task.size(); i++) {
		char c = task[i];
		if (c == '(') {
			if (++bracked == 1) continue;
		}
		else if (c == ')') {
			if (--bracked == 0) {
				first = b_content;
				ffor[first] = ffun(first);
				if (task.size() <= i + 1) break;
				oper = task[i + 1];
				second = task.substr(i + 2);
				break;
			}
		}
		if (bracked) {
			b_content += c;
		}
	}

	return make_tuple(first, oper, second);
}

function<bool()> ffun(string task) {

	if (ffor.find(task) != ffor.end()) return ffor[task];
	if ((task.size() == 1) && fvar.find(task[0]) != fvar.end()) return [&, ch = task[0]]() {return fvar[ch];};

	auto [fs, op, sc] = deleg(task);

	if ((fs != "")) {
		if (op == '+') {
			ffor[task] = [=]() {return ffun(fs)() || ffun(sc)();};
		}
		else if (op == '*') {
			ffor[task] = [=]() {return ffun(fs)() && ffun(sc)();};
		}
		else if (op == '>') {
			ffor[task] = [=]() {return !ffun(fs)() || ffun(sc)();};
		}
	}

	else if (ffor.find(task) == ffor.end()) {
		if (task[1] == '+') {
			ffor[task] = [f = &fvar[task[0]], s = &fvar[task[2]]]() {return *f || *s;};
		}
		else if (task[1] == '*') {
			ffor[task] = [f = &fvar[task[0]], s = &fvar[task[2]]]() {return *f && *s;};
		}
		else if (task[1] == '>') {
			ffor[task] = [f = &fvar[task[0]], s = &fvar[task[2]]]() {return !*f || *s;};
		}
	}

	return ffor[task];
}

int main() {
	cout << "Help for you" << endl;
	cout << "(!a)\tnot a" << endl;
	cout << "a*b\ta and b" << endl;
	cout << "a+b\ta or b" << endl;
	cout << "a>b\ta -> b (a imp b)" << endl;

	string _task;
	getline(cin, _task);

	int nvar = 0;
	string task = "";

	for (char t : _task) {
		if (t == ' ') continue;
		if (('A' <= t && t <= 'z') && !(fvar.find(t) != fvar.end())) {
			fvar[t] = 0;
			nvar++;
		}
		task += t;
	}
	ffor.clear();
	ffor[task] = ffun(task);

	for (auto& kv : fvar) {
		auto& key = kv.first;
		cout << key << '\t';
	}
	for (auto& kv : ffor) {
		auto& key = kv.first;
		cout << key << '\t';
	}
	cout << endl;

	for (int i = 0; i < pow(2, nvar); i++) {
		int j = 0;
		for (auto& kv : fvar) {
			kv.second = (i >> j) & 1;
			cout << kv.second << '\t';
			j++;
		}

		for (auto& kv : ffor) {
			auto& key = kv.first;
			auto& val = kv.second;
			if (val) {
				cout << val();
				for (int k = 0; k < key.size(); k += 8) {
					cout << '\t';
				}
			}
		}
		cout << endl;
	}

	return 0;
}
