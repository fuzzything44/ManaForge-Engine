#pragma once
#include <Window.h>
#include <CLHandler.h>
#include <TextureLibrary.h>


class MainWindow : public Window
{
public:

	MainWindow(std::string title, WindowMode mode, uvec2 size);
	MainWindow(const char* title, WindowMode mode, uvec2 size);

	glm::mat4 viewMat;
	glm::mat4 projection;


	// 1/sclae * 2 is how many units fit vertically
	GLfloat scale = 0.1f;

	const GLfloat minScale = .00625f;
	const GLfloat maxScale = 1.f;


private:
	virtual GLint init() override;
	virtual void scroll(GLFWwindow* window, double x, double y) override;
	virtual void draw(float deltaTime) override;
	virtual void input(GLFWwindow* window, float deltaTime) override;
	virtual GLint exit() override;
	virtual GLvoid focus(GLFWwindow* window, int focused) override;
	virtual GLvoid resize(GLFWwindow* window, double x, double y);
};