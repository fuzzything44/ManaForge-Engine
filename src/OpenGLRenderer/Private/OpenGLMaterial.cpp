#include "OpenGLMaterial.h"
#include "OpenGLTexture.h"

#include <Logging.h>
#include <Helper.h>
#include <Texture.h>

GLint OpenGLMaterial::startTexUniform = -1;

OpenGLMaterial::~OpenGLMaterial()
{
	if (startTexUniform == -1)
	{
		startTexUniform = glGetUniformLocation(program, "textures");
	}
}

void OpenGLMaterial::addShaderProgramFromFile(std::string name)
{
	std::string vertexPath	= "shaders\\" + name + "vert.glsl";
	std::string fragPath	= "shaders\\" + name + "frag.glsl";

	ENG_LOGLN(std::endl << std::endl);

	// Create the shaders
	uint32 VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint32 FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = loadFileToStr(vertexPath.c_str());

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = loadFileToStr(fragPath.c_str());


	int32 Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	ENG_LOGLN("Compiling vertex Shader " << vertexPath);

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* VertexShaderErrorMessage = new char[InfoLogLength + 1]();
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		ENG_LOGLN("\n" << &VertexShaderErrorMessage[0]);
		delete[] VertexShaderErrorMessage;
	}
	else
	{
		ENG_LOGLN("Shader " << vertexPath << " Successfully Compiled\n");
	}


	// Compile Fragment Shader
	ENG_LOGLN("\nCompiling fragment Shader " << fragPath);
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* FragmentShaderErrorMessage = new char[InfoLogLength + 1]();
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, FragmentShaderErrorMessage);
		ENG_LOGLN("\n" << FragmentShaderErrorMessage);
		delete[] FragmentShaderErrorMessage;
	}
	else
	{
		ENG_LOGLN("\nShader " << fragPath << " Successfully Compiled\n");
	}


	// Link the program
	ENG_LOGLN("Linking program " << name);
	uint32 ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* ProgramErrorMessage = new char[InfoLogLength + 1]();
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, ProgramErrorMessage);
		ENG_LOGLN(ProgramErrorMessage);
		delete[] ProgramErrorMessage;
	}

	// free the sader memory -- it is already in the program
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	program = ProgramID;
}

void OpenGLMaterial::addShaderProgramFromSource(std::string shader)
{
}

void OpenGLMaterial::setTexture(uint32 ID, Texture* texture)
{
	OpenGLTexture* texGL = static_cast<OpenGLTexture*>(texture);

	textures[ID] = texGL->ID;
}



void OpenGLMaterial::use()
{
	glUseProgram(program);

	//for (uint32 i = 0; i < maxTextures; i++)
	//{
	//	glUniform1i(startTexUniform + i, i);

	//	glActiveTexture(GL_TEXTURE0 + i);
	//	glBindTexture(GL_TEXTURE_2D, textures[i]);
	//}
	glUniform1i(startTexUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

}

GLint OpenGLMaterial::operator()()
{
	return program;
}