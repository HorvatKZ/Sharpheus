#include "pch.h"
#include "OpenGL_Renderer.hpp"

#include <GL/glew.h>


namespace Sharpheus {

	OpenGL_Renderer::OpenGL_Renderer()
	{

	}


	OpenGL_Renderer::~OpenGL_Renderer()
	{

	}

	void OpenGL_Renderer::StartFrame()
	{

	}

	void OpenGL_Renderer::EndFrame()
	{

	}

	void OpenGL_Renderer::DrawQuad(const Point& begin, const Point& end)
	{
		glBegin(GL_QUADS);

		glTexCoord2f(0.f, 0.f);
		glVertex2f(begin.x, begin.y);

		glTexCoord2f(1.f, 0.f);
		glVertex2f(end.x, begin.y);

		glTexCoord2f(1.f, 1.f);
		glVertex2f(end.x, end.y);

		glTexCoord2f(0.f, 1.f);
		glVertex2f(begin.x, end.y);

		glEnd();
	}

}