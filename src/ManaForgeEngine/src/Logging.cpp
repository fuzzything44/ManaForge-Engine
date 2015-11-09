#include "EnginePCH.h"

#include "Logging.h"

namespace logdetail
{

ENGINE_API boost::iostreams::stream<sink_t>* log_base::str = nullptr;

ENGINE_API std::wofstream* sink_t::file = nullptr;
}
