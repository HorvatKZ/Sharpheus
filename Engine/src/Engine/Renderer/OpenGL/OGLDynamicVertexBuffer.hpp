#pragma once

#include "pch.h"
#include "OGLVertex.hpp"


namespace Sharpheus::OpenGL {

	class DynamicVertexBuffer
	{
	public:
		DynamicVertexBuffer(class ShaderProgram& shader);
		virtual ~DynamicVertexBuffer();

		void Init();
		void PushQuad(Vertex vertices[4], GLuint texID = SPH_OGL_ID_NONE);
		void Flush();

	private:
		class ShaderProgram& shader;
		GLuint vao = SPH_OGL_ID_NONE, vbo = SPH_OGL_ID_NONE, ib = SPH_OGL_ID_NONE;
		Vertex* vertices = nullptr;
		uint32 maxQuadCount = SPH_OGL_MAX_QUADS_IN_BATCH, count = 0, nextFreeTexSlot = 0;
		std::unordered_map<GLuint, GLuint> texIDs;
		GLint texSlotNum = 0;

		GLuint* GenerateIndices(GLuint maxQuadCount);
		inline void SetIndex(GLuint* indices, uint32 i, uint32 j, uint32 index) { indices[6 * i + j] = 4 * i + index; }
		inline GLuint GetSlotFromTexID(GLuint texID) {
			auto it = texIDs.find(texID);
			if (it != texIDs.end()) {
				return it->second;
			}
			
			if (nextFreeTexSlot == texSlotNum) {
				Flush();
			}

			texIDs.insert(std::make_pair(texID, nextFreeTexSlot));
			return nextFreeTexSlot++;
		}
	};

}