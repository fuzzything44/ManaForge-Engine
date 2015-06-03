#include "OpenGLMaterial.h"
#include "OpenGLTexture.h"

#include <Logging.h>
#include <Helper.h>
#include <Texture.h>


OpenGLMaterial::OpenGLMaterial(const std::string& name)
{
	addShaderProgramFromFile(name);

	startTexUniform = glGetUniformLocation(program, "textures");
}

OpenGLMaterial::~OpenGLMaterial()
{

	glDeleteProgram(program);

}

void OpenGLMaterial::addShaderProgramFromFile(std::string name)
{
	std::string vertexPath	= "shaders\\" + name + "vert.glsl";
	std::string fragPath	= "shaders\\" + name + "frag.glsl";

	ENG_LOGLN(std::endl << std::endl);

	// Create the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = loadFileToStr(vertexPath.c_str());

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = loadFileToStr(fragPath.c_str());


	int32 Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	ENG_LOGLN("Compiling vertex Shader " << vertexPath);

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, nullptr);
	glCompileShader(vertexShader);

	// Check Vertex Shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* VertexShaderErrorMessage = new char[InfoLogLength + 1]();
		glGetShaderInfoLog(vertexShader, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
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
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(fragmentShader);

	// Check Fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* FragmentShaderErrorMessage = new char[InfoLogLength + 1]();
		glGetShaderInfoLog(fragmentShader, InfoLogLength, nullptr, FragmentShaderErrorMessage);
		ENG_LOGLN("\n" << FragmentShaderErrorMessage);
		delete[] FragmentShaderErrorMessage;
	}
	else
	{
		ENG_LOGLN("\nShader " << fragPath << " Successfully Compiled\n");
	}


	// Link the program
	ENG_LOGLN("Linking program " << name);
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		char* ProgramErrorMessage = new char[InfoLogLength + 1]();
		glGetProgramInfoLog(program, InfoLogLength, nullptr, ProgramErrorMessage);
		ENG_LOGLN(ProgramErrorMessage);
		delete[] ProgramErrorMessage;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void OpenGLMaterial::addShaderProgramFromSource(std::string shader)
{
}

void OpenGLMaterial::setTexture(uint32 ID, Texture* texture)
{
	textures[ID] = texture->getID();


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