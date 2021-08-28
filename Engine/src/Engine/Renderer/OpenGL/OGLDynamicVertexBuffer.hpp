#pragma once

#include "pch.h"
#include "OGLVertex.hpp"


namespace Sharpheus::OpenGL {

	class DynamicVertexBuffer
	{
	public:
		DynamicVertexBuffer(class ShaderProgram& shader);
		virtual ~DynamicVertexBuffer();

		void Init(GLuint maxQuadCount);
		void PushQuad(Vertex vertices[4], GLuint texID = OGL_ID_NONE);
		void Flush();

	private:
		class ShaderProgram& shader;
		GLuint vao = OGL_ID_NONE, vbo = OGL_ID_NONE, ib = OGL_ID_NONE;
		Vertex* vertices = nullptr;
		GLuint* texIDs;
		uint32 count = 0, texIDSize = 1;

		GLuint* GenerateIndices(GLuint maxQuadCount);
		inline GLuint GetSlotFromTexID(GLuint texID) { texIDs[0] = texID; return 0; }
		inline void SetIndex(GLuint* indices, uint32 i, uint32 j, uint32 index) { indices[6 * i + j] = 4 * i + index; }
	};

}