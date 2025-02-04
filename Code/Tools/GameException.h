#pragma once

#include <exception>
#include <iostream>
#include <source_location>
#include "Console.h"

class GameException : public std::exception
{
public:

	GameException(const std::string& errorMessage = "Error",
	const std::source_location& location = std::source_location::current());

	const std::string& GetErrorMessage() const;
	const std::string& GetFunctionName() const;
	const std::string& GetFileName() const;
	int GetLineNumber() const;

	virtual const char* what() const noexcept override;

private:

	std::string errorMessage;
	std::string fileName;
	std::string functionName;
	int lineNumber;
};

void RenderErorr(const std::exception& exception);