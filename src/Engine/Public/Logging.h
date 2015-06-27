#pragma once
#include "Engine.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>


enum class severity_t
{
	Trace,
	Debug,
	Info,
	Warning,
	Error,
	Fatal
};

template<typename char_t, typename traits>
inline std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, severity_t sev)
{
	if (sev == severity_t::Trace) stream << "Trace";
	else if (sev == severity_t::Debug) stream << "Debug";
	else if (sev == severity_t::Info) stream << "Info";
	else if (sev == severity_t::Warning) stream << "Warning";
	else if (sev == severity_t::Error) stream << "Error";
	else if (sev == severity_t::Fatal) stream << "Fatal";
	else stream << static_cast<int>(sev);

	return stream;
	
}

typedef boost::log::sources::severity_logger_mt<severity_t> logger_t;

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(__logging_log, logger_t)
{

	boost::log::add_common_attributes();

	boost::log::register_simple_formatter_factory<severity_t, char>("Severity");

	const char* formatStr = "[%TimeStamp%] (%Severity%): %Message%";

	boost::log::add_file_log(
		boost::log::keywords::file_name = "ENG_LOG_%N.log",
		boost::log::keywords::rotation_size = 1024 * 1024 * 5,
		boost::log::keywords::format = formatStr
	);

	boost::log::add_console_log(
		std::cout,
		boost::log::keywords::format = formatStr
	);

	return logger_t();
}

inline std::string toUnqualifiedName(const char* begin)
{
	std::string begin_str = begin;

	size_t pos = begin_str.find("rpg-simulator\\src", 17);
	if (pos != std::string::npos)
	{
		pos += 17;
		
		std::string ret;
		std::copy(begin_str.begin() + pos, begin_str.end(), std::back_inserter(ret));

		return ret;
	}

	pos = begin_str.find("rpg-simulator/src", 17);
	if (pos != std::string::npos)
	{
		pos += 17;

		std::string ret;
		std::copy(begin_str.begin() + pos, begin_str.end(), std::back_inserter(ret));

		return ret;
	}

	return begin_str;
}

#define ENG_LOGLN(Severity) BOOST_LOG_SEV(::__logging_log::get(), ::severity_t::##Severity) << "{" << toUnqualifiedName(__FILE__) << ":" << __LINE__ << "} "
