#include "OpenGLRendererConfig.h"

#include "OpenGLRenderer.h"
#include "OpenGLMaterialInstance.h"
#include "OpenGLWindow.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureLibrary.h"
#include "OpenGLModel.h"
#include "OpenGLModelData.h"
#include "OpenGLMaterialSource.h"

#include <SOIL/SOIL.h>

#include <Runtime.h>
#include <Renderer.h>
#include <Helper.h>
#include <Logging.h>
#include <PhysicsSystem.h>
#include <CameraComponent.h>
#include <ParallelQueue.h>

#include <functional>
#include <algorithm>

OpenGLRenderer::OpenGLRenderer()
{
	
}

void OpenGLRenderer::init()
{

	window = std::make_unique<OpenGLWindow>();
	debugDraw = std::make_unique<OpenGLMaterialInstance>(getMaterialSource("debugdraw"));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.2f, .2f, .2f, 1.f);

	showLoadingImage();
}



OpenGLRenderer::~OpenGLRenderer()
{
	assert(models.size() == 0);

	glFinish();
}

Window& OpenGLRenderer::getWindow()
{
	check(window);

	return *window;
}

const Window& OpenGLRenderer::getWindow() const
{
	check(window);

	return *window;
}

std::unique_ptr<Model> OpenGLRenderer::newModel()
{
	return std::make_unique<OpenGLModel>(*this);
}

std::shared_ptr<Texture> OpenGLRenderer::getTexture(const path_t& name)
{
	auto iter = textures.find(name);

	std::shared_ptr<OpenGLTexture> ret;

	if (iter != textures.end())
	{
		if (iter->second.expired())
		{
			ret = std::make_shared<OpenGLTexture>(name);
			iter->second = ret;

		}
		else
		{
			ret = std::shared_ptr<OpenGLTexture>{ iter->second };
		}

		return ret;
	}

	ret = std::make_shared<OpenGLTexture>(name);

	// make another
	textures.insert(
		{ name, ret }
	);

	return ret;
}

std::shared_ptr<MaterialSource> OpenGLRenderer::getMaterialSource(const path_t& name)
{
	auto iter = matSources.find(name);

	std::shared_ptr<OpenGLMaterialSource> ret;

	if (iter != matSources.end() )
	{
		if (iter->second.expired())
		{
			ret = std::make_shared<OpenGLMaterialSource>(name);
			iter->second = ret;
			
		}
		else
		{
			ret = std::shared_ptr<OpenGLMaterialSource>{ iter->second };
		}
		
		return ret;
	}

	ret = std::make_shared<OpenGLMaterialSource>(name);
	// make another
	matSources.insert( 
		{ name, ret }
	);

	return ret;

}

std::unique_ptr<TextureLibrary> OpenGLRenderer::newTextureLibrary()
{
	return std::make_unique<OpenGLTextureLibrary>();
}

std::unique_ptr<MaterialInstance> OpenGLRenderer::newMaterial(std::shared_ptr<MaterialSource> source)
{
	
	return std::make_unique<OpenGLMaterialInstance>(source);
}

std::unique_ptr<ModelData> OpenGLRenderer::newModelData()
{
	return std::make_unique<OpenGLModelData>();
}


bool OpenGLRenderer::update(float /*deltaTime*/)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// call the draw function for all of the models
	std::for_each(models.begin(), models.end(), [](OpenGLModel* model)
		{
			model->draw();
		}
	);

	glDisable(GL_DEPTH_TEST);

	Runtime::get().moduleManager.getPhysicsSystem().drawDebugPoints();

	glEnable(GL_DEPTH_TEST);

	window->swapBuffers();
	window->pollEvents();

	bool ret = !window->shouldClose();
	return ret;
}


void OpenGLRenderer::showLoadingImage()
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	GLuint vao;
	GLuint vbo;
	GLuint texCoordBuffer;
	GLuint ebo;

	vec2 vertLocs[] = 
	{
		{ -1.f, -1.f },
		{ +1.f, -1.f },
		{ -1.f, +1.f },
		{ +1.f, +1.f }, 
	};

	vec2 texCoords[] =
	{
		{ +0.f, +1.f },
		{ +1.f, +1.f },
		{ +0.f, +0.f },
		{ +1.f, +0.f },
	};

	uvec3 elems[] =
	{
		{ 0, 1, 2 },
		{ 1, 2, 3 }, 
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, vertLocs, GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, texCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * 2, elems, GL_STATIC_DRAW);

	auto program = std::static_pointer_cast<OpenGLMaterialSource>(getMaterialSource("boilerplate"));
	auto texture = SOIL_load_OGL_texture("textures\\loading.dds", 4, 0, SOIL_FLAG_DDS_LOAD_DIRECT);
	glUseProgram(**program);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(**program, "textures"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);

	glm::mat3 mvp = glm::ortho2d(-1.f, 1.f, -1.f, 1.f);
	glUniformMatrix3fv(glGetUniformLocation(**program, "MVPmat"), 1, GL_FALSE, &mvp[0][0]);

	glUniform1i(glGetUniformLocation(**program, "renderOrder"), 1);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	// bind UV data to the element attrib array so it shows up in our sahders -- the location is  (look in shader)
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(
		1, // location 1 (see shader)
		2, // two elements per vertex (u,v)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // use the GL_ARRAY_BUFFER instead of copying on the spot
		);
	
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // we don't have to do this bc its already bound
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &ebo);

	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(**program);

	// display it
	window->swapBuffers();
	window->pollEvents();
}

void OpenGLRenderer::setCurrentCamera(CameraComponent& newCamera)
{
	currentCamera = &newCamera;
}


CameraComponent& OpenGLRenderer::getCurrentCamera()
{
	check(currentCamera);
	return *currentCamera;
}

const CameraComponent& OpenGLRenderer::getCurrentCamera() const  
{
	check(currentCamera);
	return *currentCamera;
}


void OpenGLRenderer::drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color)
{

	check(currentCamera);

	debugDraw->use();

	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());
		
	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, verts, GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_LINE_LOOP, 0, numVerts);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);



}
void OpenGLRenderer::drawDebugLine(vec2* locs, uint32 numLocs, Color color)
{

	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());

	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numLocs, locs, GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_LINE_STRIP, 0, numLocs);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

}
void OpenGLRenderer::drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color)
{
	check(currentCamera);

	debugDraw->use();

	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());

	glUniform4f(glGetUniformLocation(program, "color"), .5f * (float)color.red / 255.f, .5f * (float)color.green / 255.f, .5f * (float)color.blue / 255.f, .5f * (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, verts, GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts);

	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glDrawArrays(GL_LINE_LOOP, 0, numVerts);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

}
void OpenGLRenderer::drawDebugOutlineCircle(vec2 center, float radius, Color color)
{

	check(currentCamera);

	using k_segments = boost::mpl::int_<16>;
	const float k_increment = 2.0f * (float)M_PI / (float)k_segments::value;
	float theta = 0.0f;
	auto verts = std::array<vec2, k_segments::value>();
	for (int32 i = 0; i < k_segments::value; ++i)
	{
		verts[i] = center + radius * vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}


	debugDraw->use();
	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());

	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * k_segments::value, &verts[0], GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_LINES, 0, k_segments::value);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);
}
void OpenGLRenderer::drawDebugSolidCircle(vec2 center, float radius, Color color)
{

	check(currentCamera);

	using k_segments = boost::mpl::int_<16>;
	const float k_increment = 2.0f * (float)M_PI / (float)k_segments::value;
	float theta = 0.0f;
	auto verts = std::array<vec2, k_segments::value>();
	for (int32 i = 0; i < k_segments::value; ++i)
	{
		verts[i] = center + radius * vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}


	debugDraw->use();

	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());

	glUniform4f(glGetUniformLocation(program, "color"), .5f * (float)color.red / 255.f, .5f * (float)color.green / 255.f, .5f * (float)color.blue / 255.f, .5f * (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * (uint32)k_segments::value, &verts[0], GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_TRIANGLE_FAN, 0, (uint32)k_segments::value);

	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glDrawArrays(GL_LINE_LOOP, 0, (uint32)k_segments::value);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);
	
}
void OpenGLRenderer::drawDebugSegment(vec2 p1, vec2 p2, Color color)
{

	check(currentCamera);

	debugDraw->use();

	GLuint program = **std::static_pointer_cast<OpenGLMaterialSource>(debugDraw->getSource());

	glUniform4f(glGetUniformLocation(program, "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	auto locs = std::array<vec2, 2>();
	locs[0] = p1;
	locs[1] = p2;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 2, &locs[0], GL_STATIC_DRAW);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized 
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

}

