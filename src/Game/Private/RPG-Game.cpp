#include <Runtime.h>

void run()
{
	auto runtime = std::make_unique<Runtime>("default");

	// run the runtime. consumes the thread until it returns
	runtime->run();


}

int32 main(int argc, char** argv)
{
	run();

	return 0;

}


