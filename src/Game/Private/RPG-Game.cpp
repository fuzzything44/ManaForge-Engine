// local includes
#include "Engine.h"

#include "Actor.h"
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
		ENG_LOGLN(reason);
		return -1;
	}
	catch (std::exception& e)
	{
		ENG_LOGLN(e.what());

		return -1;
	}
}


