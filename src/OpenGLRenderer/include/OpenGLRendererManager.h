#pragma once

#include "OpenGLRendererConfig.h"

#include <CoreManager.h>
#include <Runtime.h>

#include "COpenGLModel.h"

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>


#include <glm-ortho-2d.h>

using OpenGLRendererManager_t =
	Manager
	<
		decltype(make_type_tuple<COpenGLModel>)
		, decltype(make_type_tuple<CoreManager_t>)
	>;


struct CentralWidget : QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	
	
public:
	explicit CentralWidget(QWidget* parent = 0, Qt::WindowFlags f = 0)
	{
		
		QSurfaceFormat format;
		format.setVersion(3, 3);
		format.setProfile(QSurfaceFormat::CoreProfile);
		
		setFormat(format);
		
		
	}

	virtual void initializeGL() override
	{
		initializeOpenGLFunctions();
	}
	
	virtual void paintGL() override
	{
		
	}
};

template<>
struct ManagerData<OpenGLRendererManager_t> : QMainWindow
{
	

public:
	explicit ManagerData< OpenGLRendererManager_t >(QWidget* parent = 0, Qt::WindowFlags flags = 0)
		:QMainWindow(parent, flags)
	{
		setCentralWidget(&GL);
	}
	
	CentralWidget GL;
	
	auto& getFuncs()
	{
		return GL;
	}
	
	
};

//template<>
//void initManager<>(OpenGLRendererManager_t& manager)
//{
//	manager.getManagerData().show();
//}
//
//template<>
//void beginPlayManager<>(OpenGLRendererManager_t& manager)
//{
//}
//
//template<>
//void updateManager<>(OpenGLRendererManager_t& manager)
//{
//	auto& window = manager.getManagerData();
//	auto& funs = window.getFuncs();
//
//	
//	funs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	funs.glClearColor(1.f, 0.f, 0.f, 1.f);
//	
//	// get aspect ratio
//	float aspectRatio = (float)window.width() / (float)window.height();
//
//	constexpr bool a = OpenGLRendererManager_t::isComponent(boost::hana::type_c<COpenGLModel>);
//
//	// draw the models
//	manager.runAllMatching(make_type_tuple<COpenGLModel, CPosition>,
//		[aspectRatio, &funs](COpenGLModel& model, CPosition& pos)
//	{
//		assert(model.modelData.indexBuffer);
//
//		mat3 MVPmat;
//		MVPmat = glm::ortho2d(-aspectRatio, aspectRatio, -1.f, 1.f);
//		//MVPmat = glm::translate(MVPmat, pos.value);
//		
//		auto&& material = *model.material;
//		auto&& modelData = model.modelData;
//
//		material.use(MVPmat);
//		funs.glBindBuffer(GL_ARRAY_BUFFER, modelData.locBuffer);
//		funs.glEnableVertexAttribArray(0);
//		funs.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
//        
//        
//		funs.glBindBuffer(GL_ARRAY_BUFFER, modelData.UVbuffer);
//		funs.glEnableVertexAttribArray(1);
//		funs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
//        
//        
//		funs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.indexBuffer);
//		funs.glDrawElements(GL_TRIANGLES, modelData.numTriangles * 3, GL_UNSIGNED_INT, nullptr);
//        
//		funs.glDisableVertexAttribArray(0);
//		funs.glDisableVertexAttribArray(1);
//	});
//
//	// TODO: implement buffer swapping
//	
//}
//
//template<>
//void exitManager<>(OpenGLRendererManager_t& manager)
//{
//	manager.getManagerData().~ManagerData();
//}

OpenGLRenderer_API extern OpenGLRendererManager_t* openGLRendererManager;