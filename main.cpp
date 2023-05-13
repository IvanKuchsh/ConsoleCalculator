#include "expression.h"

int main() {
	std::string str;
	std::cin >> str;
	auto result = EvaluateExpression(str);

	std::cout << result;
}
