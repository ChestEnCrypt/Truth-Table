#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;

unordered_map<char, function<bool()>> func = { {'0', []() {return 0;}}, {'1', []() {return 1;}} };
unordered_map<char, function<bool()>> fvar;
unordered_map<string, function<bool()>> ffor;

static bool _and(bool a, bool b) {
	return a && b;
}
static bool _or(bool a, bool b) {
	return a || b;
}
static bool _imp(bool a, bool b) {
	return !a || b;
}

string str(char c) {
	return "" + c;
}

function<bool()> ffun(string task);

struct deleg {
public:
	string first = "";
	string second = "";
	char oper = ' ';

	bool d(string task) {
		int bracked = 0;
		string b_content = "";
		for (int i = 0; i < task.size(); i++) {
			char c = task[i];
			if (c == '(') {
				bracked++;
				if (bracked == 1) {
					continue;
				}
			}
			else if (c == ')') {
				bracked--;
				if (bracked == 0) {
					first = b_content;
					ffor[first] = ffun(first);
					if (task.size() <= i+1) return 1;
					oper = task[i + 1];
					second = task.substr(i + 2);
					return 1;
				}
			}
			if (bracked) {
				b_content += c;
			}
		}
		return 0;
	}
};

function<bool()> ffun(string task) {

	if (ffor.find(task) != ffor.end()) return ffor[task];
	if ((task.size() == 1) && fvar.find(task[0]) != fvar.end()) return fvar[task[0]];

	deleg d = deleg();
	if (d.d(task) && (d.second != "")) {
		switch (d.oper)
		{
		case '+':
			ffor[task] = [=]() -> bool {
				return _or(ffun(d.first)(), ffun(d.second)());
			};
			break;
		case '*':
			ffor[task] = [=]() -> bool {
				return _and(ffun(d.first)(), ffun(d.second)());
			};
			break;
		case '>':
			ffor[task] = [=]() -> bool {
				return _imp(ffun(d.first)(), ffun(d.second)());
			};
			break;
		}
	}

	else if (ffor.find(task) == ffor.end()) {
		switch (task[1])
		{
		case '+':
			ffor[task] = [=]() -> bool {return _or(fvar[task[0]](), fvar[task[2]]());};
			break;
		case '*':
			ffor[task] = [=]() -> bool {return _and(fvar[task[0]](), fvar[task[2]]());};
			break;
		case '>':
			ffor[task] = [=]() -> bool {return _imp(fvar[task[0]](), fvar[task[2]]());};
			break;
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

	string _task; // task from ures (terminal), has a trah chars
	getline(cin, _task);

	int nvar = 0; // number of variables in fvar
	string task = ""; // cleared `task`

	for (char t : _task) {
		if (t == ' ') continue; // skip garbage (space)
		if (('A' <= t && t <= 'z') && !(fvar.find(t) != fvar.end())) {
			fvar[t] = []() { return 0; };
			nvar++;
		}
		task += t;
	}
	ffor.clear();
	ffun(task);
	{
		deleg d = deleg();
		if (!d.d(task)) {
			ffor[task];
		}
		for (auto& kv : ffor) {
			auto& val = kv.second;
			if (val) val();
		}
	}

	for (auto& kv : fvar) {
		auto& key = kv.first;
		auto& val = kv.second;
		cout << key << '\t';
	}
	for (auto& kv : ffor) {
		auto& key = kv.first;
		auto& val = kv.second;
		cout << key << '\t';
	}
	cout << endl;
 
	vector<char> keys;
	for (const auto& kv : fvar) keys.push_back(kv.first);
	for (int i = 0; i < pow(2, nvar); i++) {
		for (int j = 0; j < nvar; j++) {
			char key = keys[j];
			fvar[key] = [bit = (i >> j) & 1]() { return bit; };
			cout << ((i >> j) & 1) << '\t';
		}
		ffor.clear();
		ffun(task);

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

	cout << endl;
	return 0;
}
