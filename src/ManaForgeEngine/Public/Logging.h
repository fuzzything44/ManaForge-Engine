#pragma once
#include "ENGException.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>

#include <boost/date_time.hpp>

enum severity_t
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

	class sink_t
	{
	public:
		typedef char char_type;
		typedef boost::iostreams::sink_tag category;


		std::streamsize write(const char_type* s, std::streamsize n)
		{
			std::copy(s, s + n, std::ostream_iterator<char_type>(std::cout));
			std::copy(s, s + n, std::ostream_iterator<char_type>(*file));

			return n;

		}


		static void init()
		{
			file = new std::ofstream("ENGLOG.log");
		}

		static void cleanup()
		{
			delete file;
		}

	private:
		ENGINE_API static std::ofstream* file;
	};


	struct log_base
	{

		inline static void flush(){ str->flush(); }

		static void init()
		{
			sink_t::init();
			str = new boost::iostreams::stream<sink_t>(sink_t());
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
	std::ostream& operator<<(const T& member)
	{
		*str << member;
		return *str;
	}
};

template<>
struct logger<Fatal> : logdetail::log_base
{
	inline logger()
	{
		using namespace boost::posix_time;

		ptime time = second_clock::local_time();

		*str << "\n[" << time << "] (" << Fatal << ") : ";
	}

	inline ~logger() 
	{
		Stack().ShowCallstack();
		flush();
		std::exit(-1);
	}

	template <typename T>
	inline std::ostream& operator<<(const T& member)
	{
		*str << member;
		return *str;
	}
};

template<>
struct logger<Error> : logdetail::log_base
{
	inline logger()
	{
		using namespace boost::posix_time;

		ptime time = second_clock::local_time();

		*str << "\n[" << time << "] (" << Fatal << ") : ";
	}

	inline ~logger()
	{
		flush();
		throw ENGException();
	}

	template <typename T>
	inline std::ostream& operator<<(const T& member)
	{
		*str << member;
		return *str;
	}
};

