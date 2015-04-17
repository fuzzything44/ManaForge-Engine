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
	float scale = 0.1f;

	const float minScale = .00625f;
	const float maxScale = 1.f;
	float aspectRatio;

	Actor* character;


	ModuleManager man;

private:
	virtual void init() override;
	virtual void scroll(float x, float y) override;
	virtual void draw(float deltaTime) override;
	virtual void input(float deltaTime) override;
	virtual void exit() override;
	virtual void focus(int focused) override;
	virtual void resize(double x, double y);
};