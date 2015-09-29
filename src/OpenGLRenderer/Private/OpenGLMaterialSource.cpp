#include "OpenGLRendererPCH.h"

#include "OpenGLMaterialSource.h"

#include "OpenGLRenderer.h"

#include "Helper.h"

#include <thread>

OpenGLMaterialSource::OpenGLMaterialSource(OpenGLRenderer& renderer, const path_t& name)
	: renderer(renderer)
	, name(name)
{
	if (!name.empty()) init(name);
}

OpenGLMaterialSource& OpenGLMaterialSource::operator=(OpenGLMaterialSource&& other)
{
	assert(&this->renderer == &other.renderer);

	// destroy the stuff in this one
	this->~OpenGLMaterialSource();

	this->program = other.program;
	this->name = other.name;
	this->MVPUniformLocation = other.MVPUniformLocation;
	this->startTexUniform = other.startTexUniform;

	return *this;
}

OpenGLMaterialSource::~OpenGLMaterialSource()
{

	renderer.runOnRenderThreadSync([this]
		{
			glDeleteProgram(program);
		});
}

void OpenGLMaterialSource::init(const path_t& name)
{
	this->name = name;

	path_t vertexPath = L"shaders\\" + name.wstring() + L"vert.glsl";
	path_t fragPath = L"shaders\\" + name.wstring() + L"frag.glsl";
	assert(boost::filesystem::exists(vertexPath));
	assert(boost::filesystem::exists(fragPath));

	renderer.runOnRenderThreadSync([this, &vertexPath, &fragPath, &name]
		{

			// Create the shaders
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			// Read the Vertex Shader code from the file
			std::string&& VertexShaderCode = loadFileToStr(vertexPath);

			// Read the Fragment Shader code from the file
			std::string&& FragmentShaderCode = loadFileToStr(fragPath);

			int32 Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			MFLOG(Trace) << "Compiling vertex Shader " << vertexPath;

			const char* VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(vertexShader, 1, &VertexSourcePointer, nullptr);
			glCompileShader(vertexShader);

			// Check Vertex Shader
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 1) {
				auto VertexShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
				glGetShaderInfoLog(vertexShader, InfoLogLength, nullptr, VertexShaderErrorMessage.data());
				MFLOG(Error) << VertexShaderErrorMessage.data();
			}
			else
			{
				MFLOG(Trace) << "\tShader " << vertexPath << " Successfully Compiled";
			}

			// Compile Fragment Shader
			MFLOG(Trace) << "\tCompiling fragment Shader " << fragPath;
			const char* FragmentSourcePointer = FragmentShaderCode.c_str();
			glShaderSource(fragmentShader, 1, &FragmentSourcePointer, nullptr);
			glCompileShader(fragmentShader);

			// Check Fragment Shader
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 1) {
				auto FragmentShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
				glGetShaderInfoLog(fragmentShader, InfoLogLength, nullptr, FragmentShaderErrorMessage.data());
				MFLOG(Error) << FragmentShaderErrorMessage.data();
			}
			else
			{
				MFLOG(Trace) << "\tShader " << fragPath << " Successfully Compiled";
			}

			// Link the program
			MFLOG(Trace) << "\tLinking program " << name;
			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			// Check the program
			glGetProgramiv(program, GL_LINK_STATUS, &Result);
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 1) {
				auto ProgramErrorMessage = std::vector<char>(InfoLogLength + 1);
				glGetProgramInfoLog(program, InfoLogLength, nullptr, ProgramErrorMessage.data());
				MFLOG(Error) << ProgramErrorMessage.data();
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			startTexUniform = glGetUniformLocation(program, "textures");
			if (startTexUniform == -1) {
				MFLOG(Warning) << "Could not find startTexUniform in program: " << name;
			}

			MVPUniformLocation = glGetUniformLocation(program, "MVPmat");
			if (MVPUniformLocation == -1) {
				MFLOG(Warning) << "Could not find MVPUniformLocation in program: " << name;
			}

			MFLOG(Trace) << "\tSuccessfully Linked Program: " << name;
		});
}

path_t OpenGLMaterialSource::getName() const { return name; }
