#include <iostream>
#include <string>
#include <fstream>

int main()
{
	std::cout << "Enter Project Name: ";

	std::string projectName;

	std::cin >> projectName;

	std::cout << std::endl << "Enter Class Name: ";

	std::string className;

	std::cin >> className;

	std::string pathToHeader = projectName + "\\Public\\" + className + ".h";
	std::ofstream header(pathToHeader);

	header << "#pragma once \n\n";
	header << "#include \"Engine.h\"\n\n";
	header << "class " << className << "\n";
	header << "{\n\n\n";
	header << "};\n";

	std::string pathToSource = projectName + "\\Private\\" + className + ".cpp";
	std::ofstream source(pathToSource);

	source << "#include \"stdafx.h\"\n";
	source << "#include \"" << className << ".h\"\n\n";
}