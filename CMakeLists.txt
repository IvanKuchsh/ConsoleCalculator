cmake_minimum_required (VERSION 3.8)

project ("ConsoleCalculator")

add_executable (consoleCalculator 
  				"tokenizer.h" 
				"expression.h" 
				"tokenizer.cpp" 
				"expression.cpp" 
				"main.cpp")
