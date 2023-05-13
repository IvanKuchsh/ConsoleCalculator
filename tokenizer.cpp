#include "tokenizer.h"

Tokenizer::Tokenizer(std::istream* in) : in_(in), type_(TokenType::kUnknown) {
	while (!in->eof()) {
		*in_ >> token_;
		if (in_->peek() == '\n') {
			break;
		}

		if (token_ >= '0' && token_ <= '9'
			|| token_ == '-' && (in_->peek() >= '0' && in_->peek() <= '9') && flag_ == true) {
			strToken_ += token_;
			flag_ = false;
			continue;
		}
		else {
			if (!strToken_.empty()) {
				listTokens_.emplace_back(strToken_);
			}
			if (token_ == ')') {
				flag_ = false;
			}
			else {
				flag_ = true;
			}
			strToken_ = token_;
			listTokens_.emplace_back(strToken_);
			strToken_ = "";

		}
	}
	if (!strToken_.empty()) {
		listTokens_.emplace_back(strToken_);
	}
}


Tokenizer::TokenType Tokenizer::GetType(std::string& str) {
	if (str == "+" || str == "-" || str == "*" || str == "/" || str == "(" || str == ")") {
		return TokenType::kSymbol;
	}
	else {
		return TokenType::kNumber;
	}
}

double Tokenizer::GetNumber(std::string& str) {
	std::stringstream sstr(str);
	double n;
	sstr >> n;
	return n;
}

std::list<std::string> Tokenizer::GetListToken() {
	return listTokens_;
}