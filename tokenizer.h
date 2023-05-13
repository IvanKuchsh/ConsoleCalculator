#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <sstream>
#include <list>
#include <string>
#include <stack>
#include <iostream>


class Tokenizer {
public:
	Tokenizer(std::istream* in);

	enum TokenType { kUnknown, kNumber, kSymbol };

	TokenType GetType(std::string& str);

	double GetNumber(std::string& str);

	std::list<std::string> GetListToken();

private:
	std::istream* in_;

	TokenType type_;
	double number_;

	std::list<std::string> listTokens_;
	std::string strToken_;
	char token_;
	bool flag_;
};
#endif