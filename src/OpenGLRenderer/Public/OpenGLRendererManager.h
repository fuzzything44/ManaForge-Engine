#include "OpenGLRendererConfig.h"

#include <CoreManager.h>
#include <Runtime.h>

#include "COpenGLModel.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

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

}

template<>
void beginPlayManager<>(OpenGLRendererManager_t& manager)
{
	
	
	

}

template<>
void updateManager<>(OpenGLRendererManager_t& manager)
{
	// draw the models
	manager.runAllMatching<boost::mpl::vector<COpenGLModel, CPosition>>(
		[](COpenGLModel& model, CPosition& pos)
	{
		mat3 MVPmat;
		MVPmat = glm::translate(MVPmat, pos.value);
		
		auto&& material = *model.material;
		auto&& modelData = model.modelData;

		material.use(MVPmat);

		glBindBuffer(GL_VERTEX_ARRAY, modelData.locBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, sizeof(vec2), GL_FLOAT, GL_FALSE, 0, nullptr);


		glBindBuffer(GL_VERTEX_ARRAY, modelData.UVbuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, sizeof(vec2), GL_FLOAT, GL_FALSE, 0, nullptr);


		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.indexBuffer);
		//glDrawElements(GL_TRIANGLES, modelData.numTriangles * 3, GL_UNSIGNED_INT, nullptr);

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