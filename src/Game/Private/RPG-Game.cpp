#include <Runtime.h>


int32 main(int argc, char** argv)
{
	try
	{

		auto runtime = std::make_unique<Runtime>("default");

		// run the runtime. consumes the thread until it returns
		runtime->run();
	}
	catch(const ENGException& /*e*/) { }
	catch (const std::exception& /*e*/) { }
	


	return 0;

}


