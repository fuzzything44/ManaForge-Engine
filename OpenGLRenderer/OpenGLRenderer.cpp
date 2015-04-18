#include "OpenGLRendererConfig.h"
#include <OpenGLRenderer.h>
#include <Renderer.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <functional>
#include <Logging.h>

uint32 OpenGLRenderer::loadShaderProgram(std::string name)
{
	std::string vertexPath = name.append("vert.glsl");
	std::string fragPath = name.append("frag.glsl");

	ENG_LOG(std::endl << std::endl);

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
	ENG_LOG("Compiling vertex Shader " << vertexPath);

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		ENG_LOG("\n" << &VertexShaderErrorMessage[0]);
	}
	else
	{
		ENG_LOG("Shader " << vertexPath << " Successfully Compiled\n")
	}


	// Compile Fragment Shader
	ENG_LOG("\nCompiling fragment Shader " << fragPath);
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		ENG_LOG("\n" << &FragmentShaderErrorMessage[0]);
	}
	else
	{
		ENG_LOG("\nShader " << fragPath << " Successfully Compiled\n")
	}


	// Link the program
	ENG_LOG("Linking program " << name);
	uint32 ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		ENG_LOG("\n" << &ProgramErrorMessage[0]);
	}

	// free the sader memory -- it is already in the program
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

Model* OpenGLRenderer::newModel(vec2* locations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems)
{
	return new OpenGLModel(locations, UVs, elems, numVerts, numElems, this);
}

void OpenGLRenderer::init()
{
	
}

void OpenGLRenderer::update()
{
	// TODO: collision etc

	// call the draw function for all of the models
	std::for_each(models.begin(), models.end(), std::bind(&OpenGLModel::draw, std::placeholders::_1));
}

void OpenGLRenderer::setCurrentCamera(CameraComponent* newCamera)
{
	check(newCamera);

	currentCamera = newCamera;

}

void OpenGLRenderer::loadTextures(std::vector<std::string> /*textures*/)
{

}

OpenGLRenderer::~OpenGLRenderer()
{

}

CameraComponent* OpenGLRenderer::getCurrentCamera()
{
	return currentCamera;
}

extern "C" OpenGLRendererPlugin_API void registerModule(ModuleManager& mm)
{
	mm.addRenderer(new OpenGLRenderer());
}

extern "C" OpenGLRendererPlugin_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}