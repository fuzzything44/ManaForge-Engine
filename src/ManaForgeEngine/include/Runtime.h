#pragma once
#include "Engine.h"

#include <memory>

#include "CoreManager.h"
#include "TimerManager.h"
#include "ModuleHandler.h"

#include <QApplication>
#include <QTimer>

class Runtime : public QObject
{
	Q_OBJECT
	
public:
	
	ENGINE_API Runtime(int argc, char** argv);
	ENGINE_API int run();

	ENGINE_API ~Runtime();

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
	
	ENGINE_API static Runtime* runtimeObj;


	bool shouldContinue = true;

	float deltaTime;

};
