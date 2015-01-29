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
	std::vector<GLubyte> outEncoded;

	unsigned error = lodepng::encode(outEncoded, inPixels, width, height);

	if (error){

		std::cout << "encoder error" << error << ": " << lodepng_error_text(error) << std::endl;

		return;
	}

	lodepng::save_file(outEncoded, filename);
}

GLvoid encodeAndSave(GLubyte* inPixels, const GLchar* filename, GLuint width, GLuint height)
{
	std::vector<GLubyte> outEncoded;

	unsigned error = lodepng::encode(outEncoded, inPixels, width, height);

	if (error){

		std::cout << "encoder error" << error << ": " << lodepng_error_text(error) << std::endl;

		return;
	}

	lodepng::save_file(outEncoded, filename);
}


string loadFileToStr(const GLchar* filename)
{
	ifstream stream;
	stream.open(filename);

	if (stream.bad())
	{
		std::cout << "Bad stream" << std::endl;
	}

	string ret, build;
	while (std::getline(stream, build))
	{
		ret += build;
		ret += "\n";
	}

	return ret;
}


GLuint loadTexture(const GLchar* filepath, GLuint& width, GLuint& height)
{
	vector<GLubyte> img;

	decode(img, filepath, width, height);

	if (img.size() == 0)
	{
		std::cout << "Bad Image" << std::endl;
		system("pause");
		return 0;
	}

	GLuint ret;
	glGenTextures(1, &ret);

	glBindTexture(GL_TEXTURE_2D, ret);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);

	return ret;

}

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

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLvoid setActiveTexture(GLuint ID)
{
	if (ID > 31)
	{
		return;
	}

	switch (ID)
	{
	case 0:
		glActiveTexture(GL_TEXTURE0);
	case 1:
		glActiveTexture(GL_TEXTURE1);
	case 2:
		glActiveTexture(GL_TEXTURE2);
	case 3:
		glActiveTexture(GL_TEXTURE3);
	case 4:
		glActiveTexture(GL_TEXTURE4);
	case 5:
		glActiveTexture(GL_TEXTURE5);
	case 6:
		glActiveTexture(GL_TEXTURE6);
	case 7:
		glActiveTexture(GL_TEXTURE7);
	case 8:
		glActiveTexture(GL_TEXTURE8);
	case 9:
		glActiveTexture(GL_TEXTURE9);
	case 10:
		glActiveTexture(GL_TEXTURE10);
	case 11:
		glActiveTexture(GL_TEXTURE11);
	case 12:
		glActiveTexture(GL_TEXTURE12);
	case 13:
		glActiveTexture(GL_TEXTURE13);
	case 14:
		glActiveTexture(GL_TEXTURE14);
	case 15:
		glActiveTexture(GL_TEXTURE15);
	case 16:
		glActiveTexture(GL_TEXTURE16);
	case 17:
		glActiveTexture(GL_TEXTURE17);
	case 18:
		glActiveTexture(GL_TEXTURE18);
	case 19:
		glActiveTexture(GL_TEXTURE19);
	case 20:
		glActiveTexture(GL_TEXTURE20);
	case 21:
		glActiveTexture(GL_TEXTURE21);
	case 22:
		glActiveTexture(GL_TEXTURE22);
	case 23:
		glActiveTexture(GL_TEXTURE23);
	case 24:
		glActiveTexture(GL_TEXTURE24);
	case 25:
		glActiveTexture(GL_TEXTURE25);
	case 26:
		glActiveTexture(GL_TEXTURE26);
	case 27:
		glActiveTexture(GL_TEXTURE27);
	case 28:
		glActiveTexture(GL_TEXTURE28);
	case 29:
		glActiveTexture(GL_TEXTURE29);
	case 30:
		glActiveTexture(GL_TEXTURE30);
	case 31:
		glActiveTexture(GL_TEXTURE31);
	default:
		break;
	}
}


cl::Program loadProgram(const GLchar* filepath, cl::Context& context, std::vector<cl::Device>& devices, cl_int* err)
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