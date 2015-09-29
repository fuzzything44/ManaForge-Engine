#include <Runtime.h>

#include <boost/signals2/connection.hpp>

int32 main(int argc, char** argv)
{
	std::cout << sizeof(boost::signals2::connection);

	try
	{

		Runtime runtime("default");

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
