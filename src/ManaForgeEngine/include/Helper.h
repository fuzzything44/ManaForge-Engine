#pragma once
#include <Engine.h>

// standard library includes
#include <ios>

#include <boost/filesystem/fstream.hpp>

/// <summary> Loads file to string.</summary>
///
/// <param name="filename"> Filename of the file.</param>
///
/// <returns> The file to string.</returns>
inline std::string loadFileToStr(const path_t& filename)
{

	// stream for the file
	boost::filesystem::ifstream stream;
	stream.open(filename);

	// if the steam if bad then return
	if (!stream.is_open()) {
		throw("file doens't exist: " + filename.string());
	}
	// define strings for each line and the final string
	std::string ret, build;

	// while there is another line, append it to ret.
	while (std::getline(stream, build)) {
		ret += build;
		ret += "\n";
	}

	return ret;
}
