#pragma once

#include "../Renderer_API.hpp"
#include "OGLShader.hpp"
#include "OGLDynamicVertexBuffer.hpp"
#include "Engine/Events/EventListener.hpp"


namespace Sharpheus::OpenGL {

	class Renderer : public Renderer_API
	{
	public:
		Renderer();
		virtual ~Renderer();

		void StartFrame(const Point& shift) override;
		void EndFrame() override;

		void DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color) override;
		void DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color) override;
		void DrawLine(const Point& begin, const Point& end, float thickness, const Color& color) override;

		void DrawQuad(Point coords[4], Point texCoords[4], const Color& tint, uint32 texID) override;
		void DrawMonocromeQuad(Point coords[4], const Color& color) override;

		uint32 CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered) override;
		void FreeTexture(uint32 texID) override;
		bool IsValidTexture(uint32 texID) override;
		uint32 GetInvalidTexture() override;

		inline const std::string& GetVersion() override { return version; }

	private:
		ShaderProgram shader;
		DynamicVertexBuffer dynamicVB;
		Vertex temp[4];
		static std::string version;
	};

}