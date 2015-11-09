///////////////////////////////////////////
/////// START STD LIBRARY HEADERS /////////
#include <ios> // IOS

#include <iostream> // STREAMS
#include <sstream>
#include <fstream>

#include <map> // CONTAINERS
#include <list>
#include <vector>
#include <string>

#include <exception> // EXCEPTION
#include <stdexcept>

#include <functional> // MISC
#include <chrono>

#include <memory> // for smart ptrs

///////////////////////////////////////////
/////////// START BOOST HEADERS
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/archive/xml_iarchive.hpp>

#include <boost/lexical_cast.hpp>

#include <boost/property_tree/json_parser.hpp>

#include <boost/timer/timer.hpp>

#include <boost/noncopyable.hpp>

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/iteration.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/export.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>

#include <boost/date_time.hpp>

#ifdef WIN32_MEAN_AND_LEAN
#undef WIN32_MEAN_AND_LEAN
#endif
#define WIN32_MEAN_AND_LEAN 1
#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <StackWalker/StackWalker.h>
