#include "Helper.h"
#include "Logging.h"
#include <fstream>
#include <string>

std::string loadFileToStr(const char* filename)
{
	// stream for the file
	std::ifstream stream;
	stream.open(filename);

	// if the steam if bad then return
	if (stream.bad())
	{
		ENG_LOGLN("Bad stream" << std::endl);
		return nullptr;
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


//
//std::string clGetErrorString(int error)
//{
//
//	switch (error)
//	{
//	case CL_DEVICE_NOT_FOUND:
//		return "DEVICE_NOT_FOUND";
//
//	case CL_DEVICE_NOT_AVAILABLE:
//		return "DEVICE_NOT_AVAILABLE";
//
//	case CL_COMPILER_NOT_AVAILABLE:
//		return "COMPILER_NOT_AVAILABLE";
//
//	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
//		return "MEM_OBJECT_ALLOCATION_FAILURE";
//
//	case CL_OUT_OF_RESOURCES:
//		return "OUT_OF_RESOURCES";
//
//	case CL_OUT_OF_HOST_MEMORY:
//		return "OUT_OF_HOST_MEMORY";
//
//	case CL_PROFILING_INFO_NOT_AVAILABLE:
//		return "PROFILING_INFO_NOT_AVAILABLE";
//
//	case CL_MEM_COPY_OVERLAP:
//		return "MEM_COPY_OVERLAP";
//
//	case CL_IMAGE_FORMAT_MISMATCH:
//		return "IMAGE_FORMAT_MISMATCH";
//
//	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
//		return "IMAGE_FORMAT_NOT_SUPPORTED";
//
//	case CL_BUILD_PROGRAM_FAILURE:
//		return "BUILD_PROGRAM_FAILURE";
//
//	case CL_MAP_FAILURE:
//		return "MAP_FAILURE";
//
//	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
//		return "MISALIGNED_SUB_BUFFER_OFFSET";
//
//	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
//		return "EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
//
//	case CL_INVALID_VALUE:
//		return "INVALID_VALUE";
//
//	case CL_INVALID_DEVICE_TYPE:
//		return "INVALID_DEVICE_TYPE";
//
//	case CL_INVALID_PLATFORM:
//		return "INVALID_PLATFORM";
//
//	case CL_INVALID_DEVICE:
//		return "INVALID_DEVICE";
//
//	case CL_INVALID_CONTEXT:
//		return "INVALID_CONTEXT";
//
//	case CL_INVALID_QUEUE_PROPERTIES:
//		return "INVALID_QUEUE_PROPERTIES";
//
//	case CL_INVALID_COMMAND_QUEUE:
//		return "INVALID_COMMAND_QUEUE";
//
//	case CL_INVALID_HOST_PTR:
//		return "INVALID_HOST_PTR";
//
//	case CL_INVALID_MEM_OBJECT:
//		return "INVALID_MEM_OBJECT";
//
//	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
//		return "INVALID_IMAGE_FORMAT_DESCRIPTOR";
//
//	case CL_INVALID_IMAGE_SIZE:
//		return "INVALID_IMAGE_SIZE";
//
//	case CL_INVALID_SAMPLER:
//		return "INVALID_SAMPLER";
//
//	case CL_INVALID_BINARY:
//		return "INVALID_BINARY";
//
//	case CL_INVALID_BUILD_OPTIONS:
//		return "INVALID_BUILD_OPTIONS";
//
//	case CL_INVALID_PROGRAM:
//		return "INVALID_PROGRAM";
//
//	case CL_INVALID_PROGRAM_EXECUTABLE:
//		return "INVALID_PROGRAM_EXECUTABLE";
//
//	case CL_INVALID_KERNEL_NAME:
//		return "INVALID_KERNEL_NAME";
//
//	case CL_INVALID_KERNEL_DEFINITION:
//		return "INVALID_KERNEL_DEFINITION";
//
//	case CL_INVALID_KERNEL:
//		return "INVALID_KERNEL";
//
//	case CL_INVALID_ARG_INDEX:
//		return "INVALID_ARG_INDEX";
//
//	case CL_INVALID_ARG_VALUE:
//		return "INVALID_ARG_VALUE";
//
//	case CL_INVALID_ARG_SIZE:
//		return "INVALID_ARG_SIZE";
//
//	case CL_INVALID_KERNEL_ARGS:
//		return "INVALID_KERNEL_ARGS";
//
//	case CL_INVALID_WORK_DIMENSION:
//		return "INVALID_WORK_DIMENSION";
//
//	case CL_INVALID_WORK_GROUP_SIZE:
//		return "INVALID_WORK_GROUP_SIZE";
//
//	case CL_INVALID_WORK_ITEM_SIZE:
//		return "INVALID_WORK_ITEM_SIZE";
//
//	case CL_INVALID_GLOBAL_OFFSET:
//		return "INVALID_GLOBAL_OFFSET";
//
//	case CL_INVALID_EVENT_WAIT_LIST:
//		return "INVALID_EVENT_WAIT_LIST";
//
//	case CL_INVALID_EVENT:
//		return "INVALID_EVENT";
//
//	case CL_INVALID_OPERATION:
//		return "INVALID_OPERATION";
//
//	case CL_INVALID_GL_OBJECT:
//		return "INVALID_GL_OBJECT";
//
//	case CL_INVALID_BUFFER_SIZE:
//		return "INVALID_BUFFER_SIZE";
//
//	case CL_INVALID_MIP_LEVEL:
//		return "INVALID_MIP_LEVEL";
//
//	case CL_INVALID_GLOBAL_WORK_SIZE:
//		return "INVALID_GLOBAL_WORK_SIZE";
//
//	case CL_INVALID_PROPERTY:
//		return "INVALID_PROPERTY";
//
//	case CL_PLATFORM_NOT_FOUND_KHR:
//		return "PLATFORM_NOT_FOUND";
//
//	default:
//		return "Unspecified error";
//	}
//}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const vec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const ivec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec2& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec3& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z;
	return os;
}

// custom vector printing
std::ostream& operator<<(std::ostream& os, const uvec4& vec)
{
	os << "X: " << vec.x << "  Y: " << vec.y << "  X: " << vec.z << "  W: " << vec.w;
	return os;
}



std::ostream& operator<<(std::ostream& os, const mat3& mat)
{
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
{
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
