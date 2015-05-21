// local includes
#include "Engine.h"


//#include <boost/archive/polymorphic_xml_oarchive.hpp>	  // uncomment for benchmark
//#include <boost/archive/polymorphic_binary_oarchive.hpp>// uncomment for benchmark
//#include <boost/archive/polymorphic_text_oarchive.hpp>  // uncomment for benchmark
//#include <boost/archive/polymorphic_xml_iarchive.hpp>	  // uncomment for benchmark
//#include <boost/archive/polymorphic_binary_iarchive.hpp>// uncomment for benchmark
//#include <boost/archive/polymorphic_text_iarchive.hpp>  // uncomment for benchmark
//														  // uncomment for benchmark
//#include <boost/serialization/nvp.hpp>				  // uncomment for benchmark
//#include <boost/serialization/list.hpp>				  // uncomment for benchmark
//														  // uncomment for benchmark
//#include <boost/timer/timer.hpp>						  // uncomment for benchmark
														  // uncomment for benchmark
														  // uncomment for benchmark
//#include <fstream>									  // uncomment for benchmark

#include "Actor.h"
#include "Runtime.h"



int32 main()
{

	//std::list<Actor*> actors_out;
	//// randomly genererate some actors
	//for (int i = 0; i < 100000; i++)
	//{
	//	actors_out.push_back(new Actor(
	//		Transform(
	//			vec2(static_cast<float>(rand()), static_cast<float>(rand())),
	//			static_cast<float>(rand()),
	//			vec2(static_cast<float>(rand()), static_cast<float>(rand())))
	//		));
	//}

	//{
	//	std::ofstream o_stream{ "savetest.txt", std::ofstream::binary };
	//	boost::archive::polymorphic_binary_oarchive o_arch{ o_stream };



	//	o_arch << BOOST_SERIALIZATION_NVP(actors_out);

	//}

	//std::list<Actor*> actors_in;

	//{

	//	std::ifstream i_stream{ "savetest.txt", std::ifstream::binary};
	//	boost::archive::polymorphic_binary_iarchive i_arch{ i_stream };

	//	std::cout << "Press eneter to start loading\n";
	//	std::cin.get();
	//	{
	//		boost::timer::auto_cpu_timer timer;
	//		i_arch >> BOOST_SERIALIZATION_NVP(actors_in);
	//	}
	//	
	//}

	//std::cout << "Saving done\n";
	//std::cin.get();

	Runtime runtime{ "default" };

	try{
		// run the runtime. consumes the thread until it returns
		runtime.run();
	}
	catch (ENGException& e)
	{
		std::string reason = e.what();
		ENG_LOGLN(reason);
		return -1;
	}
	catch (std::exception& e)
	{
		ENG_LOGLN(e.what());

		return -1;
	}
	
}


