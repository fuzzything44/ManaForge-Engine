#pragma once
#include "Engine.h"
#include "Logging.h"

#include <exception>
#include <string>

class Stack : public StackWalker
{
public:

	Stack(int options = OptionsAll, // 'int' is by design, to combine the enum-flags
		LPCSTR szSymPath = NULL,
		DWORD dwProcessId = GetCurrentProcessId(),
		HANDLE hProcess = GetCurrentProcess()) : StackWalker(options, szSymPath, dwProcessId, hProcess){ }

	std::string operator()()
	{
		return val;
	}

protected:
	virtual void OnOutput(LPCSTR text) override
	{
		val = text;

		StackWalker::OnOutput(text);
	}

	std::string val;
};

class ENGException : std::exception
{
public:
	virtual ENGINE_API const char* what() const override;

	ENGINE_API explicit ENGException(std::string reasonIn);
	

private:
	std::string reason;
};


/// <summary> Called upon a fatal error.</summary>
#define FATAL_ERR(message) \
	throw ENGException(message)\
	/**/
