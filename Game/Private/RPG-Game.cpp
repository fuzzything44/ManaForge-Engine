// local includes
#include "Engine.h"
#include "ENGException.h"
#include "Logging.h"
#include "Runtime.h"

#include "Helper.h"

#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>


int32 main()
{
	Runtime::init();
	Runtime runtime{"default", WindowProps(RenderMode::NORMAL, WindowMode::WINDOWED, "RPG-Game", uvec2(800, 600), true)};

	try{

		// run the runtime. consumes the thread until it returns
		runtime.run();
	}
	catch (std::exception& e)
	{
		ENG_LOG(e.what());

		return -1;
	}
	
}


