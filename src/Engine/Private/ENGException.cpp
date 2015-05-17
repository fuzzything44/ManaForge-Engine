#include "ENGException.h"

#include <sstream>

ENGException::ENGException(std::string reasonIn) 
{
	std::stringstream ss;

	ss << reasonIn << "\tStack: ";

	Stack s;
	s.ShowCallstack();
}

const char* ENGException::what() const
{
	return reason.c_str();
}




void Stack::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
{
		
	std::string name = entry.name;

	if (eType == firstEntry)
	{
		hasPrintedMain = false;
		wasLastExternal = false;
	}

	if (name == "StackWalker::ShowCallstack") return;

	if (!hasPrintedMain)
	{

		if ((name[0] == 's' && name[1] == 't' && name[2] == 'd') ||
			name == "RtlInitializeExceptionChain" || name == "BaseThreadInitThunk" ||
			name == "mainCRTStartup" || name == "__tmainCRTStartup" ||
			name == "")
		{
			if (!wasLastExternal)
			{
				ENG_LOGLN("External code");
			}
			wasLastExternal = true;

		}
		else
		{
			std::string startFile = entry.lineFileName;
			std::string fileOut;

			char lastThree[4] = "   ";

			// go up to source
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
			for (int i = 0; i < floor(fileOut.size() / 2); ++i)
			{
				std::swap(fileOut[i], fileOut[fileOut.size() - i - 1]);
			}

			ENG_LOGLN("Name: " << entry.undFullName << "\n\tFile: " << fileOut << "\n\t\tLine: " << entry.lineNumber);



			wasLastExternal = false;
		}

	}

	if (name == "main" && !hasPrintedMain) hasPrintedMain = true;
}

void Stack::OnOutput(LPCSTR text)
{
	StackWalker::OnOutput(text);
}