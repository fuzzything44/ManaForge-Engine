#include "EnginePCH.h"

#include "Engine.h"
#include "ENGException.h"

#include <sstream>

#include <boost/algorithm/string.hpp>



//////////////////////////////////////////////////////////////////////////////

ENGException::ENGException(const std::string& reasonIn) 
{

}

const char* ENGException::what() const noexcept { return ""; }
