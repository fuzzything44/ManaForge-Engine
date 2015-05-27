#pragma once
#include "Engine.h"
#include "Logging.h"

#include <exception>

class Stack : public StackWalker
{
public:

	explicit Stack(int options = OptionsAll, // 'int' is by design, to combine the enum-flags
		LPCSTR szSymPath = nullptr,
		DWORD dwProcessId = GetCurrentProcessId(),
		HANDLE hProcess = GetCurrentProcess()) : StackWalker(options, szSymPath, dwProcessId, hProcess){ }

protected:
	bool hasPrintedMain;
	bool wasLastExternal;

	ENGINE_API virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;

	ENGINE_API virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size,
		DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion) override;

	

};

class ENGException : std::exception
{
public:
	virtual ENGINE_API const char* what() const override;

	ENGINE_API explicit ENGException(std::string reasonIn);
	
};


/// <summary> Called upon a fatal error.</summary>
#define FATAL_ERR(message) \
	throw ENGException(message)\
	/**/
