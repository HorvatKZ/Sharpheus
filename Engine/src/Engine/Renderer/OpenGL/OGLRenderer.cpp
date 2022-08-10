#include "pch.h"
#include "OGLRenderer.hpp"
#include "Engine/GameObjects/Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Sharpheus::OpenGL {

	std::string Renderer::version;

	Renderer::Renderer()
		: mainShader("shader.glsl.vert", "shader.glsl.frag"), dynamicVB(mainShader)
	{
		SPH_VERIFY(glewInit() == GLEW_OK, "Error during GLEW initialization!");
		version = (char*)glGetString(GL_VERSION);
		dynamicVB.Init();
		SPH_INFO("OpenGL Batch Renderer created. Running OpenGL v{0}", version);
	}


	Renderer::~Renderer()
	{
	}


	void Renderer::StartFrame(const Point& shift)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		mainShader.Use();
		glClearColor(bgColor.GetRed(), bgColor.GetGreen(), bgColor.GetBlue(), 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, camera->GetWidth(), camera->GetHeight());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		Transform camTrafo = camera->GetWorldTrafo();
		glm::mat4 mat = glm::translate(
			glm::rotate(
				glm::scale(
					glm::translate(
						glm::mat4(1),
						glm::vec3(2 * shift.x / camera->GetOGWidth(), -2 * shift.y / camera->GetOGHeight(), 0)
					),
					glm::vec3(2 / (camTrafo.scale.x * camera->GetOGWidth()), -2 / (camTrafo.scale.y * camera->GetOGHeight()), 1)
				),
				camTrafo.rot,
				glm::vec3(0, 0, 1)
			),
			glm::vec3(-camTrafo.pos.x, -camTrafo.pos.y, 0)
		);
		mainShader.SetUniform("viewTrafo", mat);
	}


	void Renderer::EndFrame()
	{
		dynamicVB.Flush();
		glFlush();
	}


	void Renderer::SetSecondary(bool isSecondary)
	{
		dynamicVB.SetSecondary(isSecondary);
	}


	void Renderer::DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color)
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


	void Renderer::DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color)
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


	void Renderer::DrawLine(const Point& begin, const Point& end, float thickness, const Color& color)
	{
		float halfThickness = thickness / 2;
		Point unit = (begin - end).Normalize();
		Point shift(unit.y * halfThickness, -unit.x * halfThickness);
		Point coords[] = {
			begin + shift,
			begin - shift,
			end - shift,
			end + shift
		};
		DrawMonocromeQuad(coords, color);
	}


	void Renderer::DrawQuad(const Point coords[4], const Point texCoords[4], const Color& tint, uint32 texID)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");
		
		glm::vec4 color = tint.ToVec4();
		temp[0] = Vertex(coords[0].ToVec2(), texCoords[0].ToVec2(), color);
		temp[1] = Vertex(coords[1].ToVec2(), texCoords[1].ToVec2(), color);
		temp[2] = Vertex(coords[2].ToVec2(), texCoords[2].ToVec2(), color);
		temp[3] = Vertex(coords[3].ToVec2(), texCoords[3].ToVec2(), color);

		dynamicVB.PushQuad(temp, texID);
	}

	
	void Renderer::DrawMonocromeQuad(const Point coords[4], const Color& color)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glm::vec4 col = color.ToVec4();
		glm::vec2 tex(0, 0);
		temp[0] = Vertex(coords[0].ToVec2(), tex, col);
		temp[1] = Vertex(coords[1].ToVec2(), tex, col);
		temp[2] = Vertex(coords[2].ToVec2(), tex, col);
		temp[3] = Vertex(coords[3].ToVec2(), tex, col);

		dynamicVB.PushQuad(temp);
	}


	void Renderer::DrawCircle(const Point coords[4], const Color& tint)
	{
		DrawCirclePart(coords, ::Sharpheus::Renderer::GetFullTexCoords(), tint);
	}


	void Renderer::DrawCirclePart(const Point coords[4], const Point texCoords[4], const Color& tint)
	{
		SPH_ASSERT(camera != nullptr, "No camera attached to the renderer");

		glm::vec4 color = tint.ToVec4();
		temp[0] = Vertex(coords[0].ToVec2(), texCoords[0].ToVec2(), color);
		temp[1] = Vertex(coords[1].ToVec2(), texCoords[1].ToVec2(), color);
		temp[2] = Vertex(coords[2].ToVec2(), texCoords[2].ToVec2(), color);
		temp[3] = Vertex(coords[3].ToVec2(), texCoords[3].ToVec2(), color);

		dynamicVB.PushCircle(temp);
	}


	uint32 Renderer::CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered)
	{
		if (data == nullptr) {
			SPH_ERROR("OpenGL Error: Cannot create texture from nullptr data");
			return SPH_OGL_ID_NONE;
		}

		GLuint texID;
		glGenTextures(1, &texID);
		if (texID == SPH_OGL_ID_NONE) {
			GLenum err = glGetError();
			SPH_ERROR("OpenGL Error: {0} - {1}. Could not create texture", err, gluErrorString(err));
		}
		glBindTexture(GL_TEXTURE_2D, texID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

		GLint sampling = filtered ? GL_LINEAR : GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return texID;
	}


	void Renderer::FreeTexture(uint32 texID)
	{
		if (texID != SPH_OGL_ID_NONE) {
			glDeleteTextures(1, &texID);
		}
	}


	bool Renderer::IsValidTexture(uint32 texID)
	{
		return texID != SPH_OGL_ID_NONE;
	}


	uint32 Renderer::GetInvalidTexture()
	{
		return SPH_OGL_ID_NONE;
	}

}