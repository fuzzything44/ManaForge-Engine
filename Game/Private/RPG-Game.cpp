// local includes
#include "Engine.h"
#include "ENGException.h"
#include "Logging.h"
#include "Runtime.h"

#include "Helper.h"

#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>

#include <boost/timer/timer.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

int32 main()
{

	std::list<Actor*> actors_out;
	// randomly genererate some actors
	for (int i = 0; i < 100000; i++)
	{
		actors_out.push_back(new Actor(
			Transform(
				vec2(static_cast<float>(rand()), static_cast<float>(rand())),
				static_cast<float>(rand()),
				vec2(static_cast<float>(rand()), static_cast<float>(rand()))),
			true));
	}

	{
		std::ofstream o_stream{ "savetest.txt" };
		boost::archive::binary_oarchive o_arch{ o_stream };



		o_arch << BOOST_SERIALIZATION_NVP(actors_out);

	}

	std::list<Actor*> actors_in;

	{

		std::ifstream i_stream{ "savetest.txt" };
		boost::archive::binary_iarchive i_arch{ i_stream };

		std::cout << "Press eneter to start loading\n";
		std::cin.get();
		{
			boost::timer::auto_cpu_timer timer;
			i_arch >> BOOST_SERIALIZATION_NVP(actors_in);
		}
		
	}

	std::cout << "Saving done\n";
	std::cin.get();

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


