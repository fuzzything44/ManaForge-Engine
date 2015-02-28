#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

#include <GLTools/glew.h>
#include <GLTools/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.inl>

#include <CLHandler.h>
#include <TextureLibrary.h>
#include <Window.h>
