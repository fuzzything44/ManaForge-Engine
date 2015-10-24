#include "EnginePCH.h"

#include "Runtime.h"
#include "ChangeDirectory.h"
#include "Logging.h"

Runtime::Runtime()
{
	changeDir();
	logdetail::log_base::init();

	coreManager = CoreManager_t::factory();

}

Runtime::~Runtime() { logdetail::log_base::cleanup(); }

void Runtime::run()
{
	

	{
		// get the callbacks
		auto initCallbacks = getModuleManager().getInitCallbacks();

		for (auto& callback : initCallbacks) {
			if (!callback) {
				MFLOG(Warning) << "init callback empty.";
			}

			callback();
		}
		MFLOG(Trace) << "init completed.";
	}

	// load the world
	world = std::unique_ptr<World>{getModuleManager().spawnClass<World>("DefaultWorld", "DefaultWorld")};
	world->init("default"); // load the test world

	// let the input manager know of the window
	auto&& window = getRenderer().getWindow();
	getInputManager().setWindow(*window);

	// spawn the new playercontrollers and pawns
	controller = world->makePlayerController();
	pawn = world->makePawn();
	// assert(controller); TODO: Core Classes
	assert(pawn);

	// set initial tick
	clock::time_point lastTick = clock::now();

	bool shouldContinue = true;

	do
	{

		// calculate tick time
		clock::time_point currentTick = clock::now();
		std::chrono::duration<float> delta_duration = currentTick - lastTick;
		deltaTime = delta_duration.count();

		lastTick = currentTick;

		getInputManager().update();
		getTimerManager().update();

		auto&& window = getRenderer().getWindow();

		// recieve the update callbacks
		auto updateCallbacks = getModuleManager().getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks) {
			if (!callback) {
				MFLOG(Warning) << "Update callback empty";
			}
			else if (!callback(deltaTime))
			{
				shouldContinue = false;
			}
		}

	} while (shouldContinue);
}
