#include "Helper.h"

std::string loadFileToStr(const char* filename)
{STACK
	// stream for the file
	std::ifstream stream;
	stream.open(filename);

	// if the steam if bad then return
	if (stream.bad())
	{
		ENG_LOG("Bad stream" << std::endl);
		return NULL;
	}
	// define strings for each line and the final string
	std::string ret, build;

	// while there is another line, append it to ret.
	while (std::getline(stream, build))
	{
		ret += build;
		ret += "\n";
	}

	return ret;
}

// Load Shaders from the files defined
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{STACK

	ENG_LOG(std::endl << std::endl);

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = loadFileToStr(vertex_file_path);


	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = loadFileToStr(fragment_file_path);


	GLint Result = GL_FALSE;
	int InfoLogLength;
	

	// Compile Vertex Shader
	ENG_LOG("\nCompiling Shader " << vertex_file_path);

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		ENG_LOG("\n" << &VertexShaderErrorMessage[0]);
	}
	ENG_LOG("\nShader " << vertex_file_path <<  " Compiled\n")


	// Compile Fragment Shader
	ENG_LOG("\nCompiling Shader " << fragment_file_path);
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		ENG_LOG("\n" << &FragmentShaderErrorMessage[0]);
	}

	ENG_LOG("\nShader " << fragment_file_path << " Compiled\n")


	// Link the program
	ENG_LOG("Linking program\n");
	GLuint ProgramID = glCreateProgram();
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


GLuint loadDDS(const char* imagepath)
{STACK

	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, imagepath, "rb");
	if (fp == nullptr)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	auto height = reinterpret_cast<unsigned int>(&(header[8]));
	auto width = reinterpret_cast<unsigned int>(&(header[12]));
	auto linearSize = reinterpret_cast<unsigned int>(&(header[16]));
	auto mipMapCount = reinterpret_cast<unsigned int>(&(header[24]));
	auto fourCC = reinterpret_cast<unsigned int>(&(header[80]));

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
}

std::string clGetErrorString(int error)
{STACK

	switch (error)
	{
	case CL_DEVICE_NOT_FOUND:
		return "DEVICE_NOT_FOUND";

	case CL_DEVICE_NOT_AVAILABLE:
		return "DEVICE_NOT_AVAILABLE";

	case CL_COMPILER_NOT_AVAILABLE:
		return "COMPILER_NOT_AVAILABLE";

	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		return "MEM_OBJECT_ALLOCATION_FAILURE";

	case CL_OUT_OF_RESOURCES:
		return "OUT_OF_RESOURCES";

	case CL_OUT_OF_HOST_MEMORY:
		return "OUT_OF_HOST_MEMORY";

	case CL_PROFILING_INFO_NOT_AVAILABLE:
		return "PROFILING_INFO_NOT_AVAILABLE";

	case CL_MEM_COPY_OVERLAP:
		return "MEM_COPY_OVERLAP";

	case CL_IMAGE_FORMAT_MISMATCH:
		return "IMAGE_FORMAT_MISMATCH";

	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		return "IMAGE_FORMAT_NOT_SUPPORTED";

	case CL_BUILD_PROGRAM_FAILURE:
		return "BUILD_PROGRAM_FAILURE";

	case CL_MAP_FAILURE:
		return "MAP_FAILURE";

	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		return "MISALIGNED_SUB_BUFFER_OFFSET";

	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
		return "EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";

	case CL_INVALID_VALUE:
		return "INVALID_VALUE";

	case CL_INVALID_DEVICE_TYPE:
		return "INVALID_DEVICE_TYPE";

	case CL_INVALID_PLATFORM:
		return "INVALID_PLATFORM";

	case CL_INVALID_DEVICE:
		return "INVALID_DEVICE";

	case CL_INVALID_CONTEXT:
		return "INVALID_CONTEXT";

	case CL_INVALID_QUEUE_PROPERTIES:
		return "INVALID_QUEUE_PROPERTIES";

	case CL_INVALID_COMMAND_QUEUE:
		return "INVALID_COMMAND_QUEUE";

	case CL_INVALID_HOST_PTR:
		return "INVALID_HOST_PTR";

	case CL_INVALID_MEM_OBJECT:
		return "INVALID_MEM_OBJECT";

	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		return "INVALID_IMAGE_FORMAT_DESCRIPTOR";

	case CL_INVALID_IMAGE_SIZE:
		return "INVALID_IMAGE_SIZE";

	case CL_INVALID_SAMPLER:
		return "INVALID_SAMPLER";

	case CL_INVALID_BINARY:
		return "INVALID_BINARY";

	case CL_INVALID_BUILD_OPTIONS:
		return "INVALID_BUILD_OPTIONS";

	case CL_INVALID_PROGRAM:
		return "INVALID_PROGRAM";

	case CL_INVALID_PROGRAM_EXECUTABLE:
		return "INVALID_PROGRAM_EXECUTABLE";

	case CL_INVALID_KERNEL_NAME:
		return "INVALID_KERNEL_NAME";

	case CL_INVALID_KERNEL_DEFINITION:
		return "INVALID_KERNEL_DEFINITION";

	case CL_INVALID_KERNEL:
		return "INVALID_KERNEL";

	case CL_INVALID_ARG_INDEX:
		return "INVALID_ARG_INDEX";

	case CL_INVALID_ARG_VALUE:
		return "INVALID_ARG_VALUE";

	case CL_INVALID_ARG_SIZE:
		return "INVALID_ARG_SIZE";

	case CL_INVALID_KERNEL_ARGS:
		return "INVALID_KERNEL_ARGS";

	case CL_INVALID_WORK_DIMENSION:
		return "INVALID_WORK_DIMENSION";

	case CL_INVALID_WORK_GROUP_SIZE:
		return "INVALID_WORK_GROUP_SIZE";

	case CL_INVALID_WORK_ITEM_SIZE:
		return "INVALID_WORK_ITEM_SIZE";

	case CL_INVALID_GLOBAL_OFFSET:
		return "INVALID_GLOBAL_OFFSET";

	case CL_INVALID_EVENT_WAIT_LIST:
		return "INVALID_EVENT_WAIT_LIST";

	case CL_INVALID_EVENT:
		return "INVALID_EVENT";

	case CL_INVALID_OPERATION:
		return "INVALID_OPERATION";

	case CL_INVALID_GL_OBJECT:
		return "INVALID_GL_OBJECT";

	case CL_INVALID_BUFFER_SIZE:
		return "INVALID_BUFFER_SIZE";

	case CL_INVALID_MIP_LEVEL:
		return "INVALID_MIP_LEVEL";

	case CL_INVALID_GLOBAL_WORK_SIZE:
		return "INVALID_GLOBAL_WORK_SIZE";

	case CL_INVALID_PROPERTY:
		return "INVALID_PROPERTY";

	case CL_PLATFORM_NOT_FOUND_KHR:
		return "PLATFORM_NOT_FOUND";

	default:
		return "Unspecified error";
	}
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec2& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec3& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec4& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec2& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec3& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec4& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec2& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec3& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec4& vec)
{STACK
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



std::ostream& operator<<(std::ostream& os, const mat3& mat)
{STACK
	auto originalPrecison = os.precision();

	os.precision(4);
	os.setf(std::ios::fixed, std::ios::floatfield); // floatfield set to fixed

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			os << mat[x][y] << " ";
		}
		os << std::endl;
	}

	// reset the precision
	os.precision(originalPrecison);
	return os;
}

std::ostream& operator<<(std::ostream& os, const mat4& mat)
{STACK
	auto originalPrecison = os.precision();

	os.precision(4);
	os.setf(std::ios::fixed, std::ios::floatfield); // floatfield set to fixed

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			os << mat[x][y] << " ";
		}
		os << std::endl;
	}

	// reset the precision
	os.precision(originalPrecison);
	return os;
}