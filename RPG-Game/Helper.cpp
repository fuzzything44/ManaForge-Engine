#include "Helper.h"

using std::vector;
using std::cout;
using std::string;
using std::ifstream;



GLvoid decode(std::vector<GLubyte>& image, const GLchar* filename, GLuint& width, GLuint& height)
{

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

}

GLvoid encodeAndSave(const std::vector<GLubyte>& inPixels, const GLchar* filename, GLuint width, GLuint height)
{
	// the encoded version of the original
	std::vector<GLubyte> outEncoded;

	// encodes it and then saves the error into error
	GLint error = lodepng::encode(outEncoded, inPixels, width, height);

	// if the encoder failed, then return.
	if (error){

		std::cout << "encoder error" << error << ": " << lodepng_error_text(error) << std::endl;

		return;
	}
	
	// save the file
	lodepng::save_file(outEncoded, filename);
}

GLvoid encodeAndSave(const GLubyte* inPixels, const GLchar* filename, GLuint width, GLuint height)
{
	// the encoded version of the original
	std::vector<GLubyte> outEncoded;

	// encodes it and then saves the error into error
	unsigned error = lodepng::encode(outEncoded, inPixels, width, height);

	// if the encoder failed, then return.
	if (error){

		std::cout << "encoder error" << error << ": " << lodepng_error_text(error) << std::endl;

		return;
	}

	// save the file
	lodepng::save_file(outEncoded, filename);
}


string loadFileToStr(const GLchar* filename)
{
	// stream for the file
	ifstream stream;
	stream.open(filename);

	// if the steam if bad then return
	if (stream.bad())
	{
		std::cout << "Bad stream" << std::endl;
		return NULL;
	}
	// define strings for each line and the final string
	string ret, build;

	// while there is another line, append it to ret.
	while (std::getline(stream, build))
	{
		ret += build;
		ret += "\n";
	}

	return ret;
}


GLuint loadTexture(const GLchar* filepath, GLuint& width, GLuint& height)
{
	// image vector.
	vector<GLubyte> img;

	// decodes the image to img
	decode(img, filepath, width, height);

	// if the image is empty return
	if (img.size() == 0)
	{
		std::cout << "Bad Image" << std::endl;
		system("pause");
		return 0;
	}
	// return value
	GLuint ret;

	// gen textures
	glGenTextures(1, &ret);

	// bind the ret to GL_TEXTURE_2D so everyting usin GL_TEXTURE_2D referrs to ret
	glBindTexture(GL_TEXTURE_2D, ret);

	// set parameters. Current filtering techunique is GL_LINEAR http://www.arcsynthesis.org/gltut/Texturing/Tut15%20Magnification.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// copy the data to the texture associated with ret. 
	// format is RGBA internally and externally, and the size is unsigned char, which is unsigned byte
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);

	return ret;

}

// Load Shaders from the files defined
GLuint LoadShaders(const GLchar * vertex_file_path, const GLchar * fragment_file_path){

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
	printf("Compiling shader : %s\n", vertex_file_path);

	GLchar const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	GLchar const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<GLchar> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<GLchar> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	// free the sader memory -- it is already in the program
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

cl::Program loadCLProgram(const GLchar* filepath, cl::Context& context, std::vector<cl::Device>& devices, cl_int* err)
{

	// load kernel
	std::string source = loadFileToStr("kernels.cl");

	cl::Program program(context, source);
	*err = CL_SUCCESS;
	*err = program.build(devices);

	if (err != CL_SUCCESS){
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << "\t" << *err << "\n";
	}


	return program;
}