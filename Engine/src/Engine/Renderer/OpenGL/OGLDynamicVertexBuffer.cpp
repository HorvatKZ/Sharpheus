#include "pch.h"
#include "OGLDynamicVertexBuffer.hpp"
#include "OGLShader.hpp"

#define SPH_OGL_SETVAOATTRIBUTE(field, ind, type, n) \
	glEnableVertexAttribArray(ind); \
	glVertexAttribPointer(ind, n, type, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, field)); 

namespace Sharpheus::OpenGL {

	DynamicVertexBuffer::DynamicVertexBuffer(ShaderProgram& shader)
		: shader(shader)
	{
	}


	DynamicVertexBuffer::~DynamicVertexBuffer()
	{
		if (vao != OGL_ID_NONE) {
			glDeleteVertexArrays(1, &vao);
		}

		if (vbo != OGL_ID_NONE) {
			glDeleteBuffers(1, &vbo);
		}

		if (ib != OGL_ID_NONE) {
			glDeleteBuffers(1, &ib);
		}

		delete[] vertices;
		delete[] texIDs;
	}


	void DynamicVertexBuffer::Init(GLuint maxQuadCount)
	{
		SPH_LOG("Creating Dynamic Vertex Buffer");
		glGenVertexArrays(1, &vao);
		SPH_ASSERT(vao != OGL_ID_NONE, "OpenGL error: Could not generate VAO");

		glGenBuffers(1, &vbo);
		SPH_ASSERT(vao != OGL_ID_NONE, "OpenGL error: Could not generate VBO");

		glGenBuffers(1, &ib);
		SPH_ASSERT(vao != OGL_ID_NONE, "OpenGL error: Could not generate IB");

		if (vao != OGL_ID_NONE && vbo != OGL_ID_NONE && ib != OGL_ID_NONE) {
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * maxQuadCount, nullptr, GL_DYNAMIC_DRAW);

			SPH_OGL_SETVAOATTRIBUTE(pos, 0, GL_FLOAT, 2);
			SPH_OGL_SETVAOATTRIBUTE(tex, 1, GL_FLOAT, 2);
			SPH_OGL_SETVAOATTRIBUTE(col, 2, GL_FLOAT, 4);
			SPH_OGL_SETVAOATTRIBUTE(slot, 3, GL_UNSIGNED_INT, 1);

			GLuint* indices = GenerateIndices(maxQuadCount);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * maxQuadCount, (void*)indices, GL_STATIC_DRAW);
			delete[] indices;

			glBindVertexArray(OGL_ID_NONE);

			vertices = new Vertex[maxQuadCount];
			texIDs = new GLuint[1];
		}
	}


	void DynamicVertexBuffer::PushQuad(Vertex vertices[4], GLuint texID)
	{
		GLuint slot = (texID == OGL_ID_NONE) ? 0 : (GetSlotFromTexID(texID) + 1);
		vertices[0].slot = slot;
		vertices[1].slot = slot;
		vertices[2].slot = slot;
		vertices[3].slot = slot;
		memcpy(this->vertices + 4 * count, vertices, 4 * sizeof(Vertex));
		++count;
	}


	void DynamicVertexBuffer::Flush()
	{
		shader.Use();
		for (uint8 i = 0; i < texIDSize; ++i) {
			glBindTextureUnit(i, texIDs[i]);
		}
		glBindVertexArray(vao);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * count, vertices);
		glDrawElements(GL_TRIANGLES, 6 * count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(OGL_ID_NONE);
		shader.Unuse();
		count = 0;
	}


	GLuint* DynamicVertexBuffer::GenerateIndices(GLuint maxQuadCount)
	{
		GLuint* indices = new GLuint[6 * maxQuadCount];
		for (uint32 i = 0; i < maxQuadCount; ++i) {
			SetIndex(indices, i, 0, 0);
			SetIndex(indices, i, 1, 2);
			SetIndex(indices, i, 2, 1);
			SetIndex(indices, i, 3, 0);
			SetIndex(indices, i, 4, 3);
			SetIndex(indices, i, 5, 2);
		}
		return indices;
	}

}