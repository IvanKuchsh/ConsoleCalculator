#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "tokenizer.h"
#include <iostream>
#include <memory>

class Expression {
public:
	virtual ~Expression();
	virtual double Evaluate() const = 0;
};

class Digit : public Expression {
public:
	Digit(double d);

	double Evaluate() const override;
private:
	const double d_;
};


class Operation : public Expression {
public:
	Operation(const std::string op);

	int precedence;

	double Evaluate() const override;

	const std::string GetOp();

	void SetRight(std::unique_ptr<Expression> right);
	void SetLeft(std::unique_ptr<Expression> left);
private:
	const std::string op_;
	std::unique_ptr<Expression> left_;
	std::unique_ptr<Expression> right_;
};

std::unique_ptr<Expression> ParseExpression(Tokenizer* tok);

double EvaluateExpression(const std::string& expression);

#endif