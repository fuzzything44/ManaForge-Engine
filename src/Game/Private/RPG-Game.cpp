// local includes
#include "Engine.h"

#include "Runtime.h"
#include "Color.h"

#include <map>
#include <string>

#include <boost/serialization/map.hpp>
#include <boost/archive/xml_oarchive.hpp>

int32 main()
{	
	//{

	//	std::map<Color, std::string> assoc =
	//	{
	//		{ Color{ 0ui8, 0ui8, 0ui8, 255ui8 }, std::string{ "4" } },
	//		{ Color{ 0ui8, 255ui8, 0ui8, 255ui8 }, std::string{ "spiral" } },
	//		{ Color{ 255ui8, 255ui8, 255ui8, 255ui8 }, std::string{ "12" } },
	//		{ Color{ 255ui8, 0ui8, 0ui8, 255ui8 }, std::string{ "8" } }

	//	};

	//	std::ofstream o_str{ "test.txt" };
	//	boost::archive::xml_oarchive o_arch{ o_str };

	//	o_arch << BOOST_SERIALIZATION_NVP(assoc);

	//}

	Runtime* runtime = new Runtime("default");

	try{
		// run the runtime. consumes the thread until it returns
		runtime->run();
		delete runtime;
	}
	catch (ENGException& e)
	{
		std::string reason = e.what();
		logger<Error>() << reason;
		return -1;
	}
	catch (std::exception& e)
	{
		logger<Error>() << e.what();

		return -1;
	}
}


