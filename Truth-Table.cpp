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

bool _and(bool a, bool b) {
	return a && b;
}
bool _or(bool a, bool b) {
	return a || b;
}
bool _imp(bool a, bool b) {
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

	if (ffor[task]) return ffor[task];
	else return ffor[task] = [task]() -> bool {

		if (task.size() == 0) return 0;
		else if (task.size() == 1) return fvar[task[0]]();
		else if (task.size() == 2) {
			if (task[0] == '!') {
				return !fvar[task[1]]();
			}
		}
		else if (task.size() == 3) {
			if (('A' <= task[0] && task[0] <= 'z') && ('A' <= task[2] && task[2] <= 'z')) {
				if (task[1] == '+') return _or(fvar[task[0]](), fvar[task[2]]());
				if (task[1] == '*') return _and(fvar[task[0]](), fvar[task[2]]());
				if (task[1] == '>') return _imp(fvar[task[0]](), fvar[task[2]]());
			}
		}

		else {
			if ('A' <= task[0] && task[0] <= 'z') {
				if (task[1] == '+') {
					if (ffor[task.substr(3, task.size() - 4)]) {
						return _or(fvar[task[0]](), ffor[task.substr(3, task.size() - 4)]());
					}
				}
				if (task[1] == '*') {
					if (ffor[task.substr(3, task.size() - 4)]) {
						return _and(fvar[task[0]](), ffor[task.substr(3, task.size() - 4)]());
					}
				}
				if (task[1] == '>') {
					if (ffor[task.substr(3, task.size() - 4)]) {
						return _imp(fvar[task[0]](), ffor[task.substr(3, task.size() - 4)]());
					}
				}
			}

			for (size_t i = 0; i < task.size(); i++) {
				if (ffor[task.substr(1, i)]) {
					if (task[i + 2] == '+') {
						if ('A' <= task[i + 3] && task[i + 3] <= 'z') {
							return _or(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
						}
						else {
							return _or(ffor[task.substr(1, i)](), ffor[task.substr(i + 4, task.size() - 5 - i)]());
						}
					}
					if (task[i + 2] == '*') {
						if ('A' <= task[i + 3] && task[i + 3] <= 'z') {
							return _and(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
						}
						else {
							return _and(ffor[task.substr(1, i)](), ffor[task.substr(i + 4, task.size() - 5 - i)]());
						}
					}
					if (task[i + 2] == '>') {
						if ('A' <= task[i + 3] && task[i + 3] <= 'z') {
							return _imp(ffor[task.substr(1, i)](), fvar[task[i + 3]]());
						}
						else {
							return _imp(ffor[task.substr(1, i)](), ffor[task.substr(i + 4, task.size() - 5 - i)]());
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
	cout << "!\tnot" << endl;
	cout << "*\tand" << endl;
	cout << "+\tor" << endl;
	cout << ">\timp" << endl;

	string task;// = "a+((b+c)*(d+e))";
	getline(cin, task);

	string ttask = task;

	for (char t : ttask) {
		if ('A' <= t && t <= 'z') {
			fvar[t] = []() { return 0; };
			cout << t << endl;

			for (char& tt : ttask) {
				if (tt == t) {
					tt = ' ';
				}
			}
		}
	}

	string new_task;
	for (char t : task) {
		if (t == ' ') continue;
		new_task += t;
	}
	task = new_task;

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

	int n = fvar.size();
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
		for (auto& kv : ffor) {
			auto& key = kv.first;
			auto& val = kv.second;
			if (val) {
				cout << val();
				for (int i = 0; i < key.size(); i += 8) {
					cout << '\t';
				}
			}
		}
		cout << endl;
	}

	cout << endl;
	return 0;
}