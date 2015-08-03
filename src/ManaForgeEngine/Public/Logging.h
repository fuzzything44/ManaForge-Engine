#pragma once
#include "ENGException.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>

#include <boost/date_time.hpp>



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
	switch (sev)
	{
	case severity_t::Trace:
		stream << "Trace";
		break;
	case severity_t::Debug:
		stream << "Debug";
		break;
	case severity_t::Info:
		stream << "Info";
		break;
	case severity_t::Warning:
		stream << "Warning";
		break;
	case severity_t::Error:
		stream << "Error";
		break;
	case severity_t::Fatal:
		stream << "Fatal";
		break;
	default:
		stream << static_cast<int>(sev);
	}

	return stream;

}

// as to not croud the global namspace
namespace logdetail
{
	inline std::wstring stringToWstring(const std::string& other)
	{
		std::wstring ret(other.size(), L'#');
		mbstowcs(&ret[0], other.c_str(), other.size());

		return ret;
	}

	class sink_t
	{
	public:
		using char_type = wchar_t;
		using category	= boost::iostreams::sink_tag;


		std::streamsize write(const char_type* s, std::streamsize n)
		{
			std::copy(s, s + n, std::ostream_iterator<char_type, char_type>(std::wcout));
			std::copy(s, s + n, std::ostream_iterator<char_type, char_type>(*file));

			return n;

		}


		static void init()
		{
			file = new std::wofstream("ENGLOG.log");
		}

		static void cleanup()
		{
			delete file;
		}

	private:
		ENGINE_API static std::wofstream* file;
	};

	
	struct log_base
	{

		inline static void flush(){ str->flush(); }

		static void init()
		{
			sink_t::init();
			str = new boost::iostreams::stream<sink_t >(sink_t());
		}

		static void cleanup()
		{
			sink_t::cleanup();
			delete str;
		}

	protected:

		ENGINE_API static boost::iostreams::stream<sink_t>* str;


	};

}

template<severity_t sev>
struct logger : logdetail::log_base
{
	inline logger()
	{
		using namespace boost::posix_time;

		ptime time = second_clock::local_time();

		*str << "\n[" << time << "] (" << sev << ") : ";
	}

	inline ~logger()
	{
		flush();
	}

	template <typename T>
	std::wostream& operator<<(const T& member)
	{
		*str << member;
		return *str;
	}
};

#define MFLOG(sev) 													 \
	for(std::tuple<bool, std::stringstream> data					 \
		{ true, std::stringstream() }; std::get<0>(data); [&data]	 \
	{																 \
		std::get<0>(data) = false;									 \
		::logger<::severity_t::##sev>() <<							 \
			::logdetail::stringToWstring(::std::get<1>(data).str()); \
		if (::severity_t::##sev == ::severity_t::Fatal)				 \
		{															 \
			::std::terminate();										 \
		}															 \
		else if (::severity_t::##sev == ::severity_t::Error)		 \
		{															 \
			throw ::ENGException();									 \
		}															 \
	}())															 \
	std::get<1>(data)

// for wide chars
#define MFLOGW(sev) 												 \
	for(bool needsRun = true; needsRun; [&needsRun]					 \
	{																 \
		needsRun = false;								    		 \
		if (::severity_t::##sev == ::severity_t::Fatal)				 \
		{															 \
			::std::terminate();										 \
		}															 \
		else if (::severity_t::##sev == ::severity_t::Error)		 \
		{															 \
			throw ::ENGException();									 \
		}															 \
	}())															 \
	::logger<severity_t::##sev>()				 