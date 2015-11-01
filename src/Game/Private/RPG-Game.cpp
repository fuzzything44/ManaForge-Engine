#include <Runtime.h>

#include <boost/signals2/connection.hpp>

int32 main(int argc, char** argv)
{

	new int(2000);
	try
	{

		Runtime runtime;

		// run the runtime. consumes the thread until it returns
		runtime.run();
	}
	catch (const ENGException& /*e*/)
	{
	}
	catch (const std::exception& /*e*/)
	{
	}

	return 0;
}
