#include "OpenGLMaterialSource.h"

#include <boost/filesystem.hpp>
#include <Helper.h>

#include <QOpenGLFunctions_3_3_Core>

OpenGLMaterialSource::OpenGLMaterialSource(QOpenGLFunctions_3_3_Core& funs_, const path_t& path)
	:funs{funs_}
{
	path_t vertexPath = L"shaders\\" + path.wstring() + L"vert.glsl";
	path_t fragPath = L"shaders\\" + path.wstring() + L"frag.glsl";
	assert(boost::filesystem::exists(vertexPath));
	assert(boost::filesystem::exists(fragPath));

	// Create the shaders
	GLuint vertexShader = funs.glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = funs.glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string&& VertexShaderCode = loadFileToStr(vertexPath);

	// Read the Fragment Shader code from the file
	std::string&& FragmentShaderCode = loadFileToStr(fragPath);

	int32 Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	MFLOG("Compiling vertex Shader " << vertexPath);

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	funs.glShaderSource(vertexShader, 1, &VertexSourcePointer, nullptr);
	funs.glCompileShader(vertexShader);

	// Check Vertex Shader
	funs.glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
	funs.glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1) {
		auto VertexShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
		funs.glGetShaderInfoLog(vertexShader, InfoLogLength, nullptr, VertexShaderErrorMessage.data());
		MFLOG(VertexShaderErrorMessage.data());
	}
	else
	{
		MFLOG("\tShader " << vertexPath << " Successfully Compiled");
	}

	// Compile Fragment Shader
	MFLOG("\tCompiling fragment Shader " << fragPath);
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	funs.glShaderSource(fragmentShader, 1, &FragmentSourcePointer, nullptr);
	funs.glCompileShader(fragmentShader);

	// Check Fragment Shader
	funs.glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
	funs.glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1) {
		auto FragmentShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
		funs.glGetShaderInfoLog(fragmentShader, InfoLogLength, nullptr, FragmentShaderErrorMessage.data());
		MFLOG(FragmentShaderErrorMessage.data());
	}
	else
	{
		MFLOG("\tShader " << fragPath << " Successfully Compiled");
	}

	// Link the program
	MFLOG("\tLinking program " << path);
	program = funs.glCreateProgram();
	funs.glAttachShader(program, vertexShader);
	funs.glAttachShader(program, fragmentShader);
	funs.glLinkProgram(program);

	// Check the program
	funs.glGetProgramiv(program, GL_LINK_STATUS, &Result);
	funs.glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1) {
		auto ProgramErrorMessage = std::vector<char>(InfoLogLength + 1);
		funs.glGetProgramInfoLog(program, InfoLogLength, nullptr, ProgramErrorMessage.data());
		MFLOG(ProgramErrorMessage.data());
	}

	funs.glDeleteShader(vertexShader);
	funs.glDeleteShader(fragmentShader);

	startTexUniform = funs.glGetUniformLocation(program, "textures");
	if (startTexUniform == -1) {
		MFLOG("Could not find startTexUniform in program: " << path);
	}

	MVPUniformLocation = funs.glGetUniformLocation(program, "MVPmat");
	if (MVPUniformLocation == -1) {
		MFLOG("Could not find MVPUniformLocation in program: " << path);
	}

	MFLOG("\tSuccessfully Linked Program: " << path);
}

void OpenGLMaterialSource::destroy()
{
	funs.glDeleteProgram(program);
}