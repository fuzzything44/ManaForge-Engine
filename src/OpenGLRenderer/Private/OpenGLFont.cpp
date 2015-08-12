#include "OpenGLRendererPCH.h"

#include "OpenGLFont.h"

#include <boost/filesystem/fstream.hpp>

#include <boost/archive/xml_wiarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unordered_map.hpp>


OpenGLFont::OpenGLFont()
{
	boost::filesystem::wifstream i_stream{"drawData.txt"};

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

OpenGLCharacterData OpenGLFont::getCharacterData(wchar_t ch)
{
	auto iter = charData.find(ch);

	if (iter != charData.end()) {
		return iter->second;
	}

	MFLOG(Error) << "Cannot find char " << ch << " in font " << fontName;

	return OpenGLCharacterData();
}

std::shared_ptr<OpenGLMaterialSource> OpenGLFont::getMaterialSource()
{
	return matSource;
}
