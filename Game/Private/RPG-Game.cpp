#include "stdafx.h"

// local includes
#include "MainWindow.h"

/*************** P R O T O T Y P E S ***************************************************/
GLvoid draw(GLfloat delta);
GLint init();
GLvoid keyboard(GLFWwindow* window, GLfloat delta);
GLvoid scroll(GLFWwindow* winodw, GLdouble x, GLdouble y);
GLvoid resize(GLFWwindow* window, GLint x, GLint y);
GLint exit();


glm::mat4 viewMat;
glm::mat4 projection;


// 1/sclae * 2 is how many units fit vertically
GLfloat scale = 0.1f;

static const GLfloat minScale = .00625f;
static const GLfloat maxScale = 1.f;

cl_int initCL();

#ifdef _WIN32
int changeDir()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule == NULL)
	{
		return -293;
	}
	// When passing NULL to GetModuleHandle, it returns handle of exe itself
	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

	std::string path = ownPth;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++){


		do {
			path = path.substr(0, path.size() - 1);
		} while (path[path.size() - 1] != '\\');

	}
	return _chdir(path.append("Resource\\").c_str());

	
}
#endif

#ifdef __linux__
int changeDir()
{
	//TODO write linux code

	return 0;
}
#endif

#ifdef __APPLE__
int changeDir()
{
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0)
		printf("executable path is %s\n", path);
	else
		printf("buffer too small; need size %u\n", size);

	std::string pathStr = path;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++){

		do {
			path = pathStr.substr(0, pathStr.size() - 1);
		} while (pathStr[pathStr.size() - 1] != '\\');

	}


	chdir(pathStr.c_str());

	return 0;
}
#endif

GLint main()
{

	int err = changeDir();
	if (err != 0)
	{
		return err;
	}

	logging::init();

	MainWindow window = MainWindow("RPG-Simulator", WindowMode::WINDOWED, uvec2(800, 600));

	// run the window. consumes the thread until it returns
	return window.run();
	
}


