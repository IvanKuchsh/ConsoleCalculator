#include "expression.h"

Expression::~Expression() {};

Digit::Digit(double d) : d_(d) {};

double Digit::Evaluate() const {
	return d_;
}

Operation::Operation(const std::string op)
	: op_(op)
	, precedence(
		[op] {
			if (op == "*" || op == "/") {
				return 2;
			}
			else {
				return 1;
			}
		}())
	{}

double Operation::Evaluate() const {
	if (op_ == "*") {
		return left_->Evaluate() * right_->Evaluate();
	}
	else if (op_ == "/") {
		return left_->Evaluate() / right_->Evaluate();
	}
	else if (op_ == "+") {
		return left_->Evaluate() + right_->Evaluate();
	}
	else if (op_ == "-") {
		return left_->Evaluate() - right_->Evaluate();
	}
	return 0;
}

const std::string Operation::GetOp() {
	return op_;
}

void Operation::SetRight(std::unique_ptr<Expression> right) {
	right_ = std::move(right);
};
void Operation::SetLeft(std::unique_ptr<Expression> left) {
	left_ = std::move(left);
};


std::unique_ptr<Expression> ParseExpression(Tokenizer* tok) {

	std::list<std::string> listTokens = std::move(tok->GetListToken());
	auto token = listTokens.begin();
	auto end = listTokens.end();

	std::stack<std::unique_ptr<Expression>> outputQueue;
	std::stack<std::unique_ptr<Operation>> operations;

	std::stack<std::list<std::string>::iterator> iterators;

	auto PopOp = [&](const int precedence) {
		while (!operations.empty()
			&& operations.top()->precedence >= precedence
			&& operations.top()->GetOp() != "(") {

			std::unique_ptr<Expression> rightVal = std::move(outputQueue.top());
			outputQueue.pop();
			std::unique_ptr<Expression> leftVal;

			if (operations.top()->GetOp() == "-" && *(--iterators.top()) == "(") {
				operations.pop();
				outputQueue.emplace(std::move(std::make_unique<Digit>(-1)));
				operations.emplace(std::move(std::make_unique<Operation>("*")));
				iterators.pop();
			}

			if (operations.top()->GetOp() == "-") {
				iterators.pop();
			}

			leftVal = std::move(outputQueue.top());
			outputQueue.pop();
			auto op = std::move(operations.top());
			operations.pop();

			op->SetRight(std::move(rightVal));
			op->SetLeft(std::move(leftVal));

			outputQueue.emplace(std::move(op));
		}
	};

	while (token != end) {
		if (tok->GetType(*token) == tok->TokenType::kNumber) {
			outputQueue.emplace(std::move(std::make_unique<Digit>(tok->GetNumber(*token))));
		}
		else if (*token == "+" || *token == "-") {
			if (!operations.empty() && operations.top()->GetOp() == "(") {
				if (*token == "-") {
					iterators.emplace(token);
				}
				operations.emplace(std::move(std::make_unique<Operation>(*token)));
			}
			else {
				PopOp(1);
				if (*token == "-") {
					iterators.emplace(token);
				}
				operations.emplace(std::move(std::make_unique<Operation>(*token)));
			}
		}
		else if (*token == "*" || *token == "/") {
			if (!operations.empty() && operations.top()->GetOp() == "(") {
				operations.emplace(std::move(std::make_unique<Operation>(*token)));
			}
			else {
				PopOp(2);
				operations.emplace(std::make_unique<Operation>(*token));
			}
		}
		else if (*token == "(") {
			operations.emplace(std::make_unique<Operation>(*token));
		}
		else if (*token == ")") {
			while (operations.top()->GetOp() != "(") {
				PopOp(0);
			}
			operations.pop();
		}

		++token;
	}

	while (!operations.empty()) {
		PopOp(0);
	}

	return std::move(outputQueue.top());
}

double EvaluateExpression(const std::string& expression) {
	std::stringstream ss(expression);
	Tokenizer tok(&ss);
	auto expr = ParseExpression(&tok);

	return expr->Evaluate();
}