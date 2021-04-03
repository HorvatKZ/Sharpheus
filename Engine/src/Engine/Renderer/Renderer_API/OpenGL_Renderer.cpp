#include "pch.h"
#include "OpenGL_Renderer.hpp"
#include "Engine/GameObjects/Camera.hpp"

#include <GL/glew.h>


namespace Sharpheus {

	OpenGL_Renderer::OpenGL_Renderer()
	{
		GLenum err = glewInit();
		SPH_ASSERT(err == GLEW_OK, "Error during GLEW initialization!");
		SPH_INFO("OpenGL renderer created");
	}


	OpenGL_Renderer::~OpenGL_Renderer()
	{

	}

	void OpenGL_Renderer::StartFrame()
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glClearColor(bgColor.GetRed(), bgColor.GetGreen(), bgColor.GetBlue(), 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, camera->GetWidth(), camera->GetHeight());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		Transform camTrafo = camera->GetWorldTrafo();
		glRotatef(camTrafo.rot, 0, 0, 1);
		glScalef(2 / (camTrafo.scale.x * camera->GetWidth()), -2 / (camTrafo.scale.y * camera->GetHeight()), 1);
		glTranslatef(-camTrafo.pos.x, -camTrafo.pos.y, 0);
	}

	void OpenGL_Renderer::EndFrame()
	{
		glFlush();
	}


	void OpenGL_Renderer::DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color)
	{
		float halfThickness = thickness / 2;
		Point coords[] = {
			Point(begin, place - halfThickness),
			Point(end, place - halfThickness),
			Point(end, place + halfThickness),
			Point(begin, place + halfThickness)
		};
		DrawMonocromeQuad(coords, color);
	}


	void OpenGL_Renderer::DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color)
	{
		float halfThickness = thickness / 2;
		Point coords[] = {
			Point(place - halfThickness, begin),
			Point(place + halfThickness, begin),
			Point(place + halfThickness, end),
			Point(place - halfThickness, end)
		};
		DrawMonocromeQuad(coords, color);
	}


	void OpenGL_Renderer::DrawQuad(Point coords[4], Point texCoords[4], const Color& tint)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glColor4f(tint.GetRed(), tint.GetGreen(), tint.GetBlue(), tint.GetAlpha());
		for (uint8_t i = 0; i < 4; ++i) {
			glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex2f(coords[i].x, coords[i].y);
		}

		glEnd();
	}

	
	void OpenGL_Renderer::DrawMonocromeQuad(Point coords[4], const Color& color)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);

		glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		for (uint8_t i = 0; i < 4; ++i) {
			glVertex2f(coords[i].x, coords[i].y);
		}

		glEnd();
	}

}