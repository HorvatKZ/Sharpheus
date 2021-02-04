#include "pch.h"
#include "OpenGL_Renderer.hpp"
#include "Engine/GameObjects/Camera.hpp"

#include <GL/glew.h>


namespace Sharpheus {

	OpenGL_Renderer::OpenGL_Renderer()
	{
		Subscribe<WindowResizedEvent>(SPH_BIND(OpenGL_Renderer::ScreenResized));
		SPH_INFO("OpenGL renderer created");
	}


	OpenGL_Renderer::~OpenGL_Renderer()
	{

	}

	void OpenGL_Renderer::StartFrame()
	{
		glClearColor(0, 0, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);   // textures
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, screenWidth, screenHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	void OpenGL_Renderer::EndFrame()
	{
		glFlush();
	}


	void OpenGL_Renderer::DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glBegin(GL_QUADS);

		Point posLeftUp = camera->Project(leftUp);
		Point posRightUp = camera->Project(rightUp);
		Point posRightDown = camera->Project(rightDown);
		Point posLeftDown = camera->Project(leftDown);

		glTexCoord2f(0.f, 0.f);
		glVertex2f(posLeftUp.x, posLeftUp.y);

		glTexCoord2f(1.f, 0.f);
		glVertex2f(posRightUp.x, posRightUp.y);

		glTexCoord2f(1.f, 1.f);
		glVertex2f(posRightDown.x, posRightDown.y);

		glTexCoord2f(0.f, 1.f);
		glVertex2f(posLeftDown.x, posLeftDown.y);

		glEnd();
	}

	
	void OpenGL_Renderer::DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
		const Point& leftDown, const Color& color)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		Point posLeftUp = camera->Project(leftUp);
		Point posRightUp = camera->Project(rightUp);
		Point posRightDown = camera->Project(rightDown);
		Point posLeftDown = camera->Project(leftDown);

		glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		glVertex2f(posLeftUp.x, posLeftUp.y);
		glVertex2f(posRightUp.x, posRightUp.y);
		glVertex2f(posRightDown.x, posRightDown.y);
		glVertex2f(posLeftDown.x, posLeftDown.y);
		glColor3f(1, 1, 1);

		glEnd();
		glEnable(GL_TEXTURE_2D);
	}


	void OpenGL_Renderer::ScreenResized(const WindowResizedEvent& e)
	{
		screenWidth = e.newWidth;
		screenHeight = e.newHeight;
	}

}