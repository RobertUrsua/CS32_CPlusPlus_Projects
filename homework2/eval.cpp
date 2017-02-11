#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;


bool isValid(string& infix);
// returns true if infix is a string containing a valid
// infix expression and returns false otherwise
int prec(char oper);
// returns precedence value of operator
// higher value = higher precedence

int evaluate(string infix, const bool values[], string& postfix, bool& result)
// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)
{
	
	if (isValid(infix)) {

		// INFIX TO POSTFIX TRANSCRIPTION

		postfix = "";
		stack<char> opStack;
		for (int i = 0; i < infix.length(); i++) {
			if (isdigit(infix[i]))
				postfix += infix[i];
			else if (infix[i] == '(') {
				opStack.push(infix[i]);
			}
			else if (infix[i] == ')') {
				while (opStack.top() != '(') {
					postfix += opStack.top();
					opStack.pop();
				}
				opStack.pop();
			}
			else {
				while (!opStack.empty() && opStack.top() != '('
					&& prec(infix[i]) <= prec(opStack.top()) ) {
					postfix += opStack.top();
					opStack.pop();
				}
				opStack.push(infix[i]);
			}
		}
		while (!opStack.empty()) {
			postfix += opStack.top();
			opStack.pop();
		}

		// INFIX EVALUATION

		stack<int> opdStack;
		for (int i = 0; i < postfix.length(); i++) {
			// IF OPERAND
			if (isdigit(postfix[i])) {
				opdStack.push(postfix[i] - 48);
			}
			// IF !
			else if (postfix[i] == '!') {
				int operand1 = opdStack.top();
				opdStack.pop();
				
				for (int i = 0; i < 10; i++) {
					if (values[operand1] != values[i]) {
						opdStack.push(i);
						break;
					}
				}				
			}
			// IF & |
			else {
				int operand2 = opdStack.top();
				opdStack.pop();
				int operand1 = opdStack.top();
				opdStack.pop();
				bool result;
				if (postfix[i] == '&') {
					result = values[operand2] && values[operand1];
				} else if (postfix[i] == '|'){
					result = values[operand2] || values[operand1];
				}
				for (int i = 0; i < 10; i++) {
					if (result == values[i]) {
						opdStack.push(i);
						break;
					}		
				}
			}
		}

		result = values[opdStack.top()];

	}
	else {
		return 1;
	}

	return 0;
}


bool isValid(string& infix)
// returns true if infix is a string containing a valid
// infix expression and returns false otherwise
{

	if (!infix.length())
		return false;

	int numPar = 0; // number of open parentheses - number of closed parentheses must be 0
	int numWhite = 0; // number of white spaces in expression
	int expLength = infix.length();
	int numDigit = 0; // this must be nonzero for the expression to be valid

					  // whitespace removal to simplify life
	for (int i = 0, j = 0; i < expLength; ) {
		if (infix[i] == ' ') {
			i++;
			numWhite++;
		}
		else {
			infix[j] = infix[i];
			j++;
			i++;
		}
	}

	infix.resize(expLength - numWhite);
	expLength = infix.length();

	// expression cant start with &, ) or |
	if (infix[0] == '&' || infix[0] == ')' || infix[0] == '|')
		return false;
	// expression cant end with !, &, ( or |
	if (infix[expLength - 1] == '!' ||
		infix[expLength - 1] == '&' ||
		infix[expLength - 1] == '(' ||
		infix[expLength - 1] == '|')
		return false;


	// Check whether each symbol is validly followed by another specific symbol
	for (int i = 0; i < expLength; i++) {
		if (infix[i] == '(')
			numPar++;
		if (infix[i] == ')')
			numPar--;
		if (!numDigit)
			numDigit = 1;

		// numbers must be followed by & , ) , or |
		// ) must be followed by & , ) , or |

		// ! must be followed by numbers , ( , or !
		// & must be followed by numbers , ( , or !
		// | must be followed by numbers , ( , or !
		// ( must be followed by numbers , ( , or !

		if (i + 1 < expLength) {
			if ((isdigit(infix[i]) || infix[i] == ')')
				&& !(infix[i + 1] == '&' ||
					infix[i + 1] == ')' ||
					infix[i + 1] == '|')
				)
				return false;
			else if ((infix[i] == '!' || infix[i] == '(' || infix[i] == '&' || infix[i] == '|')
				&& !(infix[i + 1] == '(' ||
					infix[i + 1] == '!' ||
					isdigit(infix[i + 1]))
				)
				return false;

		}
	}
	if (numPar || !numDigit)
		return false;

	return true;
}

int prec(char oper)
// returns precedence value of operator
// higher value = higher precedence
{
	if (oper == '!')
		return 3;
	if (oper == '&')
		return 2;
	if (oper == '|')
		return 1;
}

int main()
{

	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;

	return 0;
}