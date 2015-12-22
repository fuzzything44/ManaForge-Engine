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
		boost::mpl::vector1<COpenGLModel>
		, boost::mpl::vector1<CoreManager_t>
	>;

	
struct CentralWidget : QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	
	CentralWidget()
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
};


template<>
struct ManagerData<OpenGLRendererManager_t> : QMainWindow
{
	ManagerData()
	{
		setCentralWidget(&GL);
	}
	
	CentralWidget GL;
	
	QOpenGLFunctions_3_3_Core& getFuncs()
	{
		return *GL.context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	}
	
};

template<>
void initManager<>(OpenGLRendererManager_t& manager)
{
}

template<>
void beginPlayManager<>(OpenGLRendererManager_t& manager)
{
	
	
	

}

template<>
void updateManager<>(OpenGLRendererManager_t& manager)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto& window = manager.getManagerData();
	auto& funs = window.getFuncs();
	
	// get aspect ratio
	float aspectRatio = (float)window.width() / (float)window.height();

	constexpr bool a = OpenGLRendererManager_t::isComponent<COpenGLModel>();

	// draw the models
	manager.runAllMatching<boost::mpl::vector<COpenGLModel, CPosition>>(
		[aspectRatio, &funs](COpenGLModel& model, CPosition& pos)
	{
		assert(model.modelData.indexBuffer);

		mat3 MVPmat;
		MVPmat = glm::ortho2d(-aspectRatio, aspectRatio, -1.f, 1.f);
		MVPmat = glm::translate(MVPmat, pos.value);
		
		auto&& material = *model.material;
		auto&& modelData = model.modelData;

		material.use(MVPmat);
		funs.glBindBuffer(GL_ARRAY_BUFFER, modelData.locBuffer);
		funs.glEnableVertexAttribArray(0);
		funs.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
        
        
		funs.glBindBuffer(GL_ARRAY_BUFFER, modelData.UVbuffer);
		funs.glEnableVertexAttribArray(1);
		funs.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), nullptr);
        
        
		funs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.indexBuffer);
		funs.glDrawElements(GL_TRIANGLES, modelData.numTriangles * 3, GL_UNSIGNED_INT, nullptr);
        
		funs.glDisableVertexAttribArray(0);
		funs.glDisableVertexAttribArray(1);
	});

	// TODO: implement buffer swapping
	
}

template<>
void exitManager<>(OpenGLRendererManager_t& manager)
{
	
}

OpenGLRenderer_API extern OpenGLRendererManager_t* openGLRendererManager;
