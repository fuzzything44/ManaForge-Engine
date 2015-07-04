#include <Runtime.h>

int32 main()
{	

	try{

		auto runtime = std::make_unique<Runtime>("default");

		// run the runtime. consumes the thread until it returns
		runtime->run();
	}
	catch (ENGException&)
	{
		return -1;
	}
	catch (std::exception& e)
	{
		logger<Error>() << e.what();

		return -1;
	}
}


