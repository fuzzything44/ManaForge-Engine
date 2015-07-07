#include "OpenGLRendererConfig.h"

#include "OpenGLRenderer.h"
#include "OpenGLMaterial.h"
#include "OpenGLWindow.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureLibrary.h"
#include "OpenGLModel.h"

#include <SOIL/SOIL.h>

#include <Runtime.h>
#include <Renderer.h>
#include <Helper.h>
#include <Logging.h>
#include <PhysicsSystem.h>
#include <CameraComponent.h>

#include <functional>
#include <algorithm>

OpenGLRenderer::OpenGLRenderer()
{
	
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

std::unique_ptr<Model> OpenGLRenderer::newModel(const ModelData& params, MeshComponent& owner)
{
	return std::make_unique<OpenGLModel>(params, owner, *this);
}

std::shared_ptr<Texture> OpenGLRenderer::newTexture(const std::string& name)
{
	auto iter = textures.find(name);

	if (iter != textures.end())
	{
		return iter->second;
	}

	// make another
	return textures.insert({ name, std::make_shared<OpenGLTexture>(name) }).first->second;
}

std::unique_ptr<TextureLibrary> OpenGLRenderer::newTextureLibrary(uint16 maxElems, uint16 individualSize)
{
	return std::make_unique<OpenGLTextureLibrary>(maxElems, individualSize);
}

std::unique_ptr<Material> OpenGLRenderer::newMaterial(const std::string& name)
{
	
	return std::make_unique<OpenGLMaterial>(name);
}

bool OpenGLRenderer::update(float /*deltaTime*/)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// call the draw function for all of the models
	std::for_each(models.begin(), models.end(), std::bind(&OpenGLModel::draw, std::placeholders::_1));

	glDisable(GL_DEPTH_TEST);

	Runtime::get().moduleManager.getPhysicsSystem().drawDebugPoints();

	glEnable(GL_DEPTH_TEST);

	window->swapBuffers();
	window->pollEvents();

	bool ret = !window->shouldClose();
	return ret;
}

void OpenGLRenderer::init()
{

	window = std::make_unique<OpenGLWindow>();
	debugDraw = std::make_unique<OpenGLMaterial>("debugdraw");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.2f, .2f, .2f, 1.f);

	showLoadingImage();
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

	auto program = OpenGLMaterial::addShaderProgramFromFile("boilerplate");
	auto texture = SOIL_load_OGL_texture("textures\\loading.dds", 4, 0, SOIL_FLAG_DDS_LOAD_DIRECT);
	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "textures"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);

	glm::mat3 mvp = glm::ortho2d(-1.f, 1.f, -1.f, 1.f);
	glUniformMatrix3fv(glGetUniformLocation(program, "MVPmat"), 1, GL_FALSE, &mvp[0][0]);

	glUniform1i(glGetUniformLocation(program, "renderOrder"), 1);

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

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &ebo);

	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);

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

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

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

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

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

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), .5f * (float)color.red / 255.f, .5f * (float)color.green / 255.f, .5f * (float)color.blue / 255.f, .5f * (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

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

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glDrawArrays(GL_LINE_LOOP, 0, numVerts);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

}
void OpenGLRenderer::drawDebugOutlineCircle(vec2 center, float radius, Color color)
{

	check(currentCamera);

	const float k_segments = 16.0f;
	const float k_increment = 2.0f * (float)M_PI / k_segments;
	float theta = 0.0f;
	vec2* verts = new vec2[(uint32)k_segments];
	for (int32 i = 0; i < k_segments; ++i)
	{
		verts[i] = center + radius * vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}


	debugDraw->use();

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * (uint32)k_segments, verts, GL_STATIC_DRAW);

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

	glDrawArrays(GL_LINES, 0, (uint32)k_segments);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

	delete[] verts;
}
void OpenGLRenderer::drawDebugSolidCircle(vec2 center, float radius, Color color)
{

	check(currentCamera);

	const float k_segments = 16.0f;
	const float k_increment = 2.0f * (float)M_PI / k_segments;
	float theta = 0.0f;
	vec2* verts = new vec2[(uint32)k_segments];
	for (int32 i = 0; i < k_segments; ++i)
	{
		verts[i] = center + radius * vec2(cosf(theta), sinf(theta));
		theta += k_increment;
	}


	debugDraw->use();

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), .5f * (float)color.red / 255.f, .5f * (float)color.green / 255.f, .5f * (float)color.blue / 255.f, .5f * (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * (uint32)k_segments, verts, GL_STATIC_DRAW);

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

	glDrawArrays(GL_TRIANGLE_FAN, 0, (uint32)k_segments);

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glDrawArrays(GL_LINE_LOOP, 0, (uint32)k_segments);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(1, &buff);
	glDeleteVertexArrays(1, &vao);

	delete[] verts;
	
}
void OpenGLRenderer::drawDebugSegment(vec2 p1, vec2 p2, Color color)
{

	check(currentCamera);

	debugDraw->use();

	glUniform4f(glGetUniformLocation((*debugDraw)(), "color"), (float)color.red / 255.f, (float)color.green / 255.f, (float)color.blue / 255.f, (float)color.alpha / 255.f);
	glUniformMatrix3fv(glGetUniformLocation((*debugDraw)(), "MVPmat"), 1, GL_FALSE, &currentCamera->getViewMat()[0][0]);

	vec2* locs = new vec2[2];
	locs[0] = p1;
	locs[1] = p2;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buff;
	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 2, locs, GL_STATIC_DRAW);

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

	delete[] locs;
}

