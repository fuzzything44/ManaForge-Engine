#pragma once

#include <Window.h>
#include <ModuleManager.h>

class MainWindow : public Window
{
public:

	/// <summary> Constructor.</summary>
	///
	/// <param name="title"> The title.</param>
	/// <param name="mode">  The mode.</param>
	/// <param name="size">  The size.</param>
	MainWindow(std::string title, WindowMode mode, uvec2 size);

	/// <summary> Constructor.</summary>
	///
	/// <param name="title"> The title.</param>
	/// <param name="mode">  The mode.</param>
	/// <param name="size">  The size.</param>
	MainWindow(const char* title, WindowMode mode, uvec2 size);

	glm::mat4 viewMat;


	// 1/sclae * 2 is how many units fit vertically
	GLfloat scale = 0.1f;

	const GLfloat minScale = .00625f;
	const GLfloat maxScale = 1.f;
	GLfloat aspectRatio;

	Actor* character;


	ModuleManager man;

private:
	virtual void init() override;
	virtual void scroll(GLFWwindow* window, double x, double y) override;
	virtual void draw(GLfloat deltaTime) override;
	virtual void input(GLFWwindow* window, GLfloat deltaTime) override;
	virtual GLint exit() override;
	virtual GLvoid focus(GLFWwindow* window, int focused) override;
	virtual GLvoid resize(GLFWwindow* window, double x, double y);
};