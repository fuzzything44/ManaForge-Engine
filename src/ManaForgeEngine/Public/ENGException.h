#pragma once
#include <exception>
#include <string>

#include "Engine.h"

class Stack : public StackWalker
{
  public:
	ENGINE_API explicit Stack(int options = OptionsAll, // 'int' is by design, to combine the enum-flags
		LPCSTR szSymPath = nullptr,
		DWORD dwProcessId = GetCurrentProcessId(),
		HANDLE hProcess = GetCurrentProcess())
		: StackWalker(options, szSymPath, dwProcessId, hProcess)
	{
	}

  protected:
	bool hasPrintedMain;
	bool wasLastExternal;

	ENGINE_API virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;

	ENGINE_API virtual void OnLoadModule(LPCSTR img,
		LPCSTR mod,
		DWORD64 baseAddr,
		DWORD size,
		DWORD result,
		LPCSTR symType,
		LPCSTR pdbName,
		ULONGLONG fileVersion) override;
};

class ENGException : std::exception
{
  public:
	ENGINE_API virtual const char* what() const override;

	ENGINE_API explicit ENGException(const std::string& reasonIn = "");
};
