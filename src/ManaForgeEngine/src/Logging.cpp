#include "EnginePCH.h"

#include "Logging.h"

#include <string>

namespace logdetail
{
using namespace std::string_literals;
DLLEXPORT std::ofstream outFile = std::ofstream{MF_RESOURCE_DIR + "/ENGLOG.log"s};
}
