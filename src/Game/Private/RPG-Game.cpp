#include <Runtime.h>

int32 main(int argc, char** argv)
{
	{
		auto runtime = std::make_unique<Runtime>("default");

		// run the runtime. consumes the thread until it returns
		runtime->run();
	}
	exit(0);

}


