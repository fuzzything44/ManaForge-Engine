#include <exception>

#include <Runtime.h>

int main(int argc, char** argv)
{
	try
	{

		Runtime runtime{argc, argv};

		//run the runtime. consumes the thread until it returns
		runtime.run();
	}
	catch (const std::exception& /*e*/)
	{
	}

	return 0;
}
