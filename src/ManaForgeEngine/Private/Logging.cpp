#include "EnginePCH.h"

#include "Logging.h"

namespace logdetail
{

	boost::iostreams::stream<sink_t>* log_base::str = nullptr;

	std::ofstream* sink_t::file = nullptr;
}
