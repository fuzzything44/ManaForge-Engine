#include "EnginePCH.h"

#include "Engine.h"
#include "ENGException.h"


#include <sstream>

#include <boost/algorithm/string.hpp>


void Stack::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
{

	std::string name = entry.name;

	if (eType == firstEntry)
	{
		hasPrintedMain = false;
		wasLastExternal = false;
	}

	if (name == "StackWalker::ShowCallstack" || name == "ENGException::ENGException") return;

	if (!hasPrintedMain)
	{

		if (boost::starts_with(name, "std") ||
			name == "RtlInitializeExceptionChain" || name == "BaseThreadInitThunk" ||
			name == "mainCRTStartup" || name == "__tmainCRTStartup" ||
			name == "")
		{
			if (!wasLastExternal)
			{
				MFLOG(Info) << "External code";
			}
			wasLastExternal = true;

		}
		else
		{
			std::string startFile = entry.lineFileName;
			std::string fileOut;

			char lastThree[4] = "   ";

			// go up to source -- this will be backwards
			for (std::string::reverse_iterator iter = startFile.rbegin(); iter != startFile.rend(); ++iter)
			{
				if (lastThree[0] == 'c' && lastThree[1] == 'r' && lastThree[2] == 's') // src backward
				{
					fileOut.push_back(*iter);
					break;
				}

				lastThree[0] = lastThree[1];
				lastThree[1] = lastThree[2];
				lastThree[2] = *iter;
				fileOut.push_back(*iter);
			}

			// reverse it
			std::reverse(fileOut.begin(), fileOut.end());

			MFLOG(Info) << "Name: " << entry.undFullName << "\n\tFile: " << fileOut << "\n\tLine: " << entry.lineNumber;

			wasLastExternal = false;
		}

	}

	if (name == "main" && !hasPrintedMain) hasPrintedMain = true;
}

void Stack::OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion)
{
	std::string name = img;

	if (!boost::starts_with(name, "C:\\Windows\\"))
	{
		MFLOG(Info) << "Module Loaded: " << mod;
	}
}

//////////////////////////////////////////////////////////////////////////////

ENGException::ENGException(const std::string& reasonIn) 
{
	
	MFLOG(Info) << reasonIn << " Stack:\n";

	Stack s;
	s.ShowCallstack();

}

const char* ENGException::what() const
{
	return "";
}
