// local includes
#include "Engine.h"


#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>

#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_text_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>

#include <boost/timer/timer.hpp>

#include "ENGException.h"
#include "Logging.h"
#include "Actor.h"
#include "Runtime.h"

#include "Helper.h"

#include <fstream>



int32 main()
{

	//std::list<Actor*> actors_out;
	//// randomly genererate some actors
	//for (int i = 0; i < 10; i++)
	//{
	//	actors_out.push_back(new Actor(
	//		Transform(
	//			vec2(static_cast<float>(rand()), static_cast<float>(rand())),
	//			static_cast<float>(rand()),
	//			vec2(static_cast<float>(rand()), static_cast<float>(rand()))),
	//		true));
	//}

	//{
	//	std::ofstream o_stream{ "savetest.txt" };
	//	boost::archive::polymorphic_binary_oarchive o_arch{ o_stream };



	//	o_arch << BOOST_SERIALIZATION_NVP(actors_out);

	//}

	//std::list<Actor*> actors_in;

	//{

	//	std::ifstream i_stream{ "savetest.txt" };
	//	boost::archive::polymorphic_binary_iarchive i_arch{ i_stream };

	//	std::cout << "Press eneter to start loading\n";

	//	{
	//		boost::timer::auto_cpu_timer timer;
	//		i_arch >> BOOST_SERIALIZATION_NVP(actors_in);
	//	}
	//	
	//}

	//std::cout << "Saving done\n";
	//std::cin.get();

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


