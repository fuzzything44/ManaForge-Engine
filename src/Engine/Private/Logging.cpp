#include "EnginePCH.h"

#include "Logging.h"

namespace logdetail
{

	boost::iostreams::stream<sink_t> log_base::str = boost::iostreams::stream<sink_t>(sink_t());

	std::ofstream sink_t::file = std::ofstream();
}
