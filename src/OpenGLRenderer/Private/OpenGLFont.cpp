#include "OpenGLRendererPCH.h"

#include "OpenGLFont.h"

#include <boost/filesystem/fstream.hpp>

#include <boost/archive/xml_wiarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unordered_map.hpp>


OpenGLFont::OpenGLFont(const std::wstring& name)
{
	if (!name.empty())
	{
		init(name);
	}
}

void OpenGLFont::init(const std::wstring & name)
{
	boost::filesystem::wifstream i_stream{ L"fonts\\" + name + L"\\drawdata.txt" };

	if (!i_stream.is_open()) MFLOG(Error) << "Failed to open draw data for font" << fontName;

	try
	{

		boost::archive::xml_wiarchive i_arch{ i_stream };


		i_arch >> boost::serialization::make_nvp("charData", charData);
	}
	catch (boost::archive::archive_exception& e)
	{
		std::cout << e.what();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what();
	}
}

