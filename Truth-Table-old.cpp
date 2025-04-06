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

function<bool()> ffun(string task) {
	int bracked = 0;
	string b_content = "";
	for (char c : task) {
		if (c == '(') {
			bracked++;
			if (bracked == 1) {
				continue;
			}
		}
		else if (c == ')') {
			bracked--;
			if (bracked == 0) {
				ffor[b_content] = ffun(b_content);
				b_content = "";
			}
		}
		if (bracked) {
			b_content += c;
		}
	}

	if (ffor.find(task) != ffor.end()) return ffor[task];
	else return ffor[task] = [task]() -> bool {

		if (task.size() == 0) return 0;
		else if (task.size() == 1) return fvar[task[0]]();
		else if (task.size() == 2) {
			if (task[0] == '!') {
				return !fvar[task[1]]();
			}
		}
		else if (task.size() == 3) {
			if ((fvar.find(task[0]) != fvar.end()) && (fvar.find(task[2]) != fvar.end())) {
				if (task[1] == '+') return _or(fvar[task[0]](), fvar[task[2]]());
				if (task[1] == '*') return _and(fvar[task[0]](), fvar[task[2]]());
				if (task[1] == '>') return _imp(fvar[task[0]](), fvar[task[2]]());
			}
		}

		else {
			if (fvar.find(task[0]) != fvar.end()) {
				for (size_t i = 0; i < task.size() - 2; i++) {
					for (size_t j = i; j < task.size() - 2; j++) {
						if (ffor.find(task.substr(i + 2, j)) != ffor.end()) {
							if (task[1] == '+') return _or(fvar[task[0]](), ffor[task.substr(i + 2, j)]());
							if (task[1] == '*') return _and(fvar[task[0]](), ffor[task.substr(i + 2, j)]());
							if (task[1] == '>') return _imp(fvar[task[0]](), ffor[task.substr(i + 2, j)]());
						}
					}
				}
			}

			for (size_t i = 0; i < task.size(); i++) {
				if (ffor.find(task.substr(1, i)) != ffor.end()) {
					if (i == task.size() - 2) return ffor[task.substr(1, i)]();
					if (fvar[task[i + 3]]) {
						if (task[i + 2] == '+') return _or(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
						if (task[i + 2] == '*') return _and(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
						if (task[i + 2] == '>') return _imp(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
					}
					for (size_t j = 0; j < task.size() - i; j++) {
						if (ffor.find(task.substr(4 + i, j)) != ffor.end()) {
							if (task[i + 2] == '+') return _or(ffor[task.substr(1, i)](), ffor[task.substr(4 + i, j)]());
							if (task[i + 2] == '*') return _and(ffor[task.substr(1, i)](), ffor[task.substr(4 + i, j)]());
							if (task[i + 2] == '>') return _imp(ffor[task.substr(1, i)](), ffor[task.substr(4 + i, j)]());
						}
					}
				}
			}
		}

		return 0;
		};

	return []() {return 0;};
}

int main() {
	cout << "Help for you" << endl;
	cout << "(!a)\tnot a" << endl;
	cout << "a*b\ta and b" << endl;
	cout << "a+b\ta or b" << endl;
	cout << "a>b\ta -> b (a imp b)" << endl;

	string task;
	getline(cin, task);

	string ttask = task;

	for (char t : ttask) {
		if (t == ' ') continue;
		if ('A' <= t && t <= 'z') {
			fvar[t] = []() { return 0; };

			for (char& tt : ttask) {
				if (tt == t) {
					tt = ' ';
				}
			}
		}
	}
	{
		string new_task;
		for (char t : task) {
			if (t == ' ') continue;
			new_task += t;
		}
		task = new_task;
	}

	// ----------------

	ffun(task);

	int sum = 0;
	for (auto& kv : fvar) {
		auto& key = kv.first;
		auto& val = kv.second;
		cout << key << '\t';
		sum++;
	}
	for (auto& kv : ffor) {
		auto& key = kv.first;
		auto& val = kv.second;
		cout << key << '\t';
	}
	cout << endl;

	size_t n = fvar.size();
	vector<char> keys;
	for (const auto& kv : fvar) keys.push_back(kv.first);
	int total = 1 << n;
	for (int i = 0; i < pow(2, sum); i++) {
		for (int j = 0; j < n; j++) {
			char key = keys[j];
			fvar[key] = [bit = (i >> j) & 1]() { return bit; };
			cout << ((i >> j) & 1) << '\t';
		}
		ffor.clear();
		ffun(task);
		for (auto it = ffor.begin(); it != ffor.end();) {
			if (it->second == nullptr) {
				it = ffor.erase(it);
			}
			else {
				++it;
			}
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

	cout << endl;
	return 0;
}