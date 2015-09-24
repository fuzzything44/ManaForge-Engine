#pragma once
#include <Renderer.h>
#include <Cacher.h>

#include <list>
#include <vector>
#include <unordered_map>
#include <thread>
#include <future>
#include <functional>
#include <tuple>
#include <utility>
#include <memory>

#include <call_from_tuple.h>

#include <boost/lockfree/spsc_queue.hpp>

#define USE_PARALLEL_RENDERER 1

class OpenGLModel;
class OpenGLMaterialInstance;
class OpenGLTexture;
class OpenGLWindow;
class OpenGLMaterialSource;
class OpenGLModelData;
class OpenGLTextBox;
class OpenGLFont;

class OpenGLRenderer : public Renderer
{

	friend class OpenGLModel;
	friend class OpenGLTextBox;

	struct RenderThread
	{
		RenderThread(std::thread&& thread)
			: isInLoop(true)
			, thread(std::move(thread))
		{
		}
		RenderThread()
			: isInLoop(true)
		{
		}

		const RenderThread& operator=(std::thread&& rightThread)
		{
			thread = std::move(rightThread);
			return *this;
		};

		operator std::thread&() { return thread; }

		std::thread& getThread() { return thread; }

		~RenderThread()
		{
			isInLoop = false;
			if (thread.joinable()) thread.join();
		}

		std::atomic<bool> isInLoop;

	private:
		std::thread thread;
	};

public:
	OpenGLRenderer();

	virtual ~OpenGLRenderer() override;

	void init();

	void showLoadingImage();

	virtual Window& getWindow() override;
	const Window& getWindow() const override;

	virtual std::unique_ptr<Model, void(*)(Model*)> newModel(uint8 renderOrder) override;
	virtual std::unique_ptr<TextBox> newTextBox() override;
	virtual Font* getFont(const path_t& name) override;
	virtual Texture* getTexture(const path_t& name) override;
	virtual MaterialSource* getMaterialSource(const path_t& name) override;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary() override;
	virtual std::unique_ptr<MaterialInstance> newMaterialInstance(MaterialSource* source) override;
	virtual std::shared_ptr<ModelData> newModelData(const std::string& name) override;
	virtual std::unique_ptr<ModelData> newModelData() override;

	virtual void deleteModel(Model* model) override;


	/// <summary> Renders the next frame. </summary>
	bool update(float deltaTime);

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent& newCamera) override;

	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;

	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) override;
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) override;

	template <typename Function, typename... Args>
	inline auto runOnRenderThreadSync(Function&& func, Args&&... args);

	// the futures from this function should NEVER be used in the render thread -- could produce deadlock
	template <typename Function, typename... Args>
	inline auto runOnRenderThreadAsync(Function&& func, Args&&... args);

	// chooses if it should be async or sync based on the thread
	template <typename Function, typename... Args>
	inline auto runOnRenderThreadAsyncOrSync(Function&& func, Args&&... args)
	{
		using retType = decltype(func(std::forward<Args>(args)...));

		if (isOnRenderThread())
		{
			std::packaged_task<retType(Args&&...)> task{ func };
			task(std::forward<Args>(args)...);
			return task.get_future();
		}
		else
		{
			return runOnRenderThreadAsync(std::forward<Function>(func), std::forward<Args>(args)...);
		}
	}

	inline bool isOnRenderThread() 
	{ 
#if USE_PARALLEL_RENDERER
		return std::this_thread::get_id() == renderThread.getThread().get_id(); 
#else
		return true;
#endif
	}

private:
	void initRenderer();
	void renderLoop();

	boost::lockfree::spsc_queue<std::function<void()>> queue;
	RenderThread renderThread;

	std::unique_ptr<OpenGLWindow> window;
	std::unique_ptr<OpenGLMaterialInstance> debugDraw;

	// then delete our atomics
	std::atomic<CameraComponent*> currentCamera;
	std::future<void> lastFrame;
	std::atomic<bool> shouldExit;

	// delete our caches and models first
	std::map<uint8, std::list<OpenGLModel*>> models;
	std::list<OpenGLTextBox*> textBoxes;

	StrongCacher<path_t, OpenGLTexture> textures;
	StrongCacher<path_t, OpenGLFont> fonts;
	StrongCacher<path_t, OpenGLMaterialSource> matSources;
	WeakCacher<std::string, OpenGLModelData> modelDataCache;

	boost::lockfree::spsc_queue<OpenGLModel*> modelsToDelete;
	boost::lockfree::spsc_queue<OpenGLModel*> modelsToAdd;
};

template <typename Function, typename... Args>
inline auto OpenGLRenderer::runOnRenderThreadSync(Function&& func, Args&&... args)
{
#if USE_PARALLEL_RENDERER
	if (isOnRenderThread()) {
		return func(std::forward<Args>(args)...);
	}

	using retType = decltype(func(Args && ...));

	std::packaged_task<retType(Args && ...)> task{func};

	queue.push([&task, &args...]
		{
			task(std::forward<Args>(args)...);
		});

	return task.get_future().get();
#else
	return func(std::forward<Args>(args)...);
#endif
}

template <typename Function, typename... Args>
inline auto OpenGLRenderer::runOnRenderThreadAsync(Function&& func, Args&&... args)
{
	using retType = decltype(func(std::forward<Args>(args)...));

#if USE_PARALLEL_RENDERER
	assert(!isOnRenderThread());

	// this needs to be a shared_ptr because the lambda needs to be copied. If the queue had emplace
	// functions...
	auto task = std::make_shared<std::packaged_task<retType(Args && ...)>>(func);

	auto ret = task->get_future(); // cache it because it will be moved from.

	queue.push([ pack = std::move(task), args = std::make_tuple(std::forward<Args>(args)...) ]() mutable
		{
			callWithTuple(*pack, args);
		});

	return ret;
#else
	std::promise<retType> a;

	a.set_value();
	func(std::forward<Args>(args)...);
	return a.get_future();
#endif

}



// ~800
