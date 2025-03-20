#include <iostream>
#include <functional>

using namespace std;

function<bool()> parsing(string task) {
	
	int quant = 0;
	int quant_b = 0;
	for (char c : task) {
		if (c == '(') {
			quant++;
		}
		else if (c == ')') {
			quant_b++;
		}
	}

	if (quant != quant_b) {
		return [&]() { cout << "Syntax Error: Brackets are not equal" << endl; return 0; };
	}

	function<bool()>* func = new function<bool()>[quant + 1];
	quant = 0;

	cout << task << endl;
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
				func[quant] = parsing(b_content);
				quant++;
				b_content = "";
			}
		}
		if (bracked) {
			b_content += c;
		}
	}
	
	return [&]() { return 0; };
}

int main() {

	{
		cout << "Help for you" << endl;
		cout << "!\tnot" << endl;
		cout << "&\tand" << endl;
		cout << "|\tor" << endl;
		cout << "^\txor" << endl;
		cout << "~\telse" << endl;
		cout << "=\txnor" << endl << endl;
	}

	//while (true)
	{
		cout << "Enter a task: ";

		string task;
		cin >> task;

		function<bool()> pars = parsing(task);
		pars();

		cout << endl << endl;
	}
}