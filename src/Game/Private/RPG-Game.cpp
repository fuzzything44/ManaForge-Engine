// local includes
#include "Engine.h"

#include "Runtime.h"

int32 main()
{	

	Runtime* runtime = new Runtime("default");

	try{
		// run the runtime. consumes the thread until it returns
		runtime->run();
		delete runtime;
	}
	catch (ENGException& e)
	{
		std::string reason = e.what();
		ENG_LOGLN(Error) << reason;
		return -1;
	}
	catch (std::exception& e)
	{
		ENG_LOGLN(Error) << e.what();

		return -1;
	}
}


