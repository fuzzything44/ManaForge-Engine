#include "OpenGLRendererConfig.h"

#include <CoreManager.h>
#include <Runtime.h>

#include "COpenGLModel.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm-ortho-2d.h>

using OpenGLRendererManager_t =
	Manager
	<
		boost::mpl::vector1<COpenGLModel>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;


template<>
struct ManagerData<OpenGLRendererManager_t>
{
	GLFWwindow* window;
};

template<>
void initManager<>(OpenGLRendererManager_t& manager)
{
	if (glfwInit() == GL_FALSE)
	{
		MFLOG(Error) << "Failed to initalize GLFW.";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow*& window = manager.getManagerData().window;

	window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);
	assert(window);


	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	{
		auto err = glewInit();
		if (err != GLEW_OK)
		{
			glfwTerminate();
			MFLOG(Error) << "Failed to initalize glew with error: " << glewGetErrorString(err);
		}
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

template<>
void beginPlayManager<>(OpenGLRendererManager_t& manager)
{
	
	
	

}

template<>
void updateManager<>(OpenGLRendererManager_t& manager)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get aspect ratio
	float aspectRatio = 0.f;
	{
		int w, h;
		glfwGetWindowSize(manager.getManagerData().window, &w, &h);

		aspectRatio = (float)w / (float)h;
	}

	constexpr bool a = OpenGLRendererManager_t::isComponent<COpenGLModel>();

	// draw the models
	manager.runAllMatching<boost::mpl::vector<COpenGLModel, CPosition>>(
		[aspectRatio](COpenGLModel& model, CPosition& pos)
	{
		assert(model.modelData.indexBuffer);

		mat3 MVPmat;
		MVPmat = glm::ortho2d(-aspectRatio, aspectRatio, -1.f, 1.f);
		MVPmat = glm::translate(MVPmat, pos.value);
		
		auto&& material = *model.material;
		auto&& modelData = model.modelData;

		material.use(MVPmat);

		glBindBuffer(GL_ARRAY_BUFFER, modelData.locBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);


		glBindBuffer(GL_ARRAY_BUFFER, modelData.UVbuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.indexBuffer);
		glDrawElements(GL_TRIANGLES, modelData.numTriangles * 3, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	});

	auto&& window = manager.getManagerData().window;

	glfwSwapBuffers(window);
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
	{
		Runtime::get().requestExit();
	}
	
}

template<>
void exitManager<>(OpenGLRendererManager_t& manager)
{
	glfwTerminate();
}

OpenGLRenderer_API extern std::weak_ptr<OpenGLRendererManager_t> openGLRendererManager;
