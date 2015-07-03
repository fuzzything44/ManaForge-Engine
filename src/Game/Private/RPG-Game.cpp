#include <Runtime.h>

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
		return -1;
	}
	catch (std::exception& e)
	{
		logger<Error>() << e.what();

		return -1;
	}
}


