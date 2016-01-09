#pragma once
#include "Engine.h"

#include <memory>

#include "CoreManager.h"
#include "TimerManager.h"
#include "ModuleHandler.h"

#include <QApplication>
#include <QTimer>

class Runtime 
{	
public:
	
	MF_API Runtime(int& argc, char**& argv);
	MF_API int run();

	MF_API ~Runtime();

	TimerManager timerManager;
	ModuleHandler moduleHandler;

	void requestExit()
	{
		shouldContinue = false;
	}

	static Runtime& get()
	{
		return *runtimeObj;
	}

	float getDeltaTime()
	{
		return deltaTime;
	}

private:
	
	QApplication application;
	QTimer timer;
	
	MF_API static Runtime* runtimeObj;


	bool shouldContinue = true;

	float deltaTime;

}; 
