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

	virtual std::unique_ptr<Model> newModel(uint8 renderOrder) override;
	virtual std::unique_ptr<TextBox> newTextBox() override;
	virtual Font* getFont(const path_t& name) override;
	virtual Texture* getTexture(const path_t& name) override;
	virtual MaterialSource* getMaterialSource(const path_t& name) override;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary() override;
	virtual std::unique_ptr<MaterialInstance> newMaterialInstance(MaterialSource* source) override;
	virtual std::shared_ptr<ModelData> newModelData(const std::string& name) override;
	virtual std::unique_ptr<ModelData> newModelData() override;

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

	template <typename Function, typename... Args>
	inline auto runOnRenderThreadAsync(Function&& func, Args&&... args);

	inline bool isOnRenderThread() { return std::this_thread::get_id() == renderThread.getThread().get_id(); }

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
};

template <typename Function, typename... Args>
inline auto OpenGLRenderer::runOnRenderThreadSync(Function&& func, Args&&... args)
{
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
}

template <typename Function, typename... Args>
inline auto OpenGLRenderer::runOnRenderThreadAsync(Function&& func, Args&&... args)
{
	assert(!isOnRenderThread());

	using retType = decltype(func(std::forward<Args>(args)...));

	// this needs to be a shared_ptr because the lambda needs to be copied. If the queue had emplace
	// functions...
	auto task = std::make_shared<std::packaged_task<retType(Args && ...)>>(func);

	auto ret = task->get_future(); // cache it because it will be moved from.

	queue.push([ pack = std::move(task), args = std::make_tuple(std::forward<Args>(args)...) ]() mutable
		{
			callWithTuple(*pack, args);
		});

	return ret;
}
