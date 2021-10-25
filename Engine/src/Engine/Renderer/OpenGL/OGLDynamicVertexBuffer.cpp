#include "pch.h"
#include "OGLDynamicVertexBuffer.hpp"
#include "OGLShader.hpp"

#define SPH_OGL_SET_VAO_ATTRIBUTE(field, ind, type, n) \
	glEnableVertexAttribArray(ind); \
	glVertexAttribPointer(ind, n, type, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, field));

#define SPH_OGL_SET_VAO_INT_ATTRIBUTE(field, ind, type, n) \
	glEnableVertexAttribArray(ind); \
	glVertexAttribIPointer(ind, n, type, sizeof(Vertex), (const void*)offsetof(Vertex, field)); 

namespace Sharpheus::OpenGL {

	DynamicVertexBuffer::DynamicVertexBuffer(ShaderProgram& shader)
		: shader(shader)
	{
	}


	DynamicVertexBuffer::~DynamicVertexBuffer()
	{
		if (vao != SPH_OGL_ID_NONE) {
			glDeleteVertexArrays(1, &vao);
		}

		if (vao2 != SPH_OGL_ID_NONE) {
			glDeleteVertexArrays(1, &vao2);
		}

		if (vbo != SPH_OGL_ID_NONE) {
			glDeleteBuffers(1, &vbo);
		}

		if (ib != SPH_OGL_ID_NONE) {
			glDeleteBuffers(1, &ib);
		}

		delete[] vertices;
	}


	void DynamicVertexBuffer::Init()
	{
		SPH_LOG("Creating Dynamic Vertex Buffer");
		glGenVertexArrays(1, &vao);
		SPH_ASSERT(vao != SPH_OGL_ID_NONE, "OpenGL error: Could not generate VAO");

		glGenBuffers(1, &vbo);
		SPH_ASSERT(vbo != SPH_OGL_ID_NONE, "OpenGL error: Could not generate VBO");

		glGenBuffers(1, &ib);
		SPH_ASSERT(ib != SPH_OGL_ID_NONE, "OpenGL error: Could not generate IB");

		if (vao != SPH_OGL_ID_NONE && vbo != SPH_OGL_ID_NONE && ib != SPH_OGL_ID_NONE) {
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * maxQuadCount, nullptr, GL_DYNAMIC_DRAW);

			SPH_OGL_SET_VAO_ATTRIBUTE(pos, 0, GL_FLOAT, 2);
			SPH_OGL_SET_VAO_ATTRIBUTE(tex, 1, GL_FLOAT, 2);
			SPH_OGL_SET_VAO_ATTRIBUTE(col, 2, GL_FLOAT, 4);
			SPH_OGL_SET_VAO_INT_ATTRIBUTE(slot, 3, GL_BYTE, 1);

			GLuint* indices = GenerateIndices(maxQuadCount);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * maxQuadCount, (void*)indices, GL_STATIC_DRAW);
			delete[] indices;

			glBindVertexArray(SPH_OGL_ID_NONE);

			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texSlotNum);
			if (texSlotNum > SPH_OGL_MAX_SUPPORTED_TEXTURE_SLOTS) {
				texSlotNum = SPH_OGL_MAX_SUPPORTED_TEXTURE_SLOTS;
			}

			vertices = new Vertex[4 * maxQuadCount];
		}
	}


	void DynamicVertexBuffer::PushQuad(Vertex vertices[4], GLuint texID)
	{
		if (count == maxQuadCount) {
			Flush();
		}

		GLuint slot = (texID == SPH_OGL_ID_NONE) ? 0 : (GetSlotFromTexID(texID) + 1);
		vertices[0].slot = slot;
		vertices[1].slot = slot;
		vertices[2].slot = slot;
		vertices[3].slot = slot;
		memcpy(this->vertices + 4 * count, vertices, 4 * sizeof(Vertex));
		++count;
	}


	void DynamicVertexBuffer::Flush()
	{
		for (auto it = texIDs.begin(); it != texIDs.end(); ++it) {
			glBindTextureUnit(it->second, it->first);
		}

		shader.Use();
		glBindVertexArray(isSecondary ? vao2 : vao);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * count, vertices);
		glDrawElements(GL_TRIANGLES, 6 * count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(SPH_OGL_ID_NONE);
		shader.Unuse();

		count = 0;
		nextFreeTexSlot = 0;
		texIDs.clear();
	}


	void DynamicVertexBuffer::SetSecondary(bool isSecondary)
	{
		this->isSecondary = isSecondary;
		if (isSecondary) {
			SPH_LOG("Creating and using Secondary Dynamic Vertex Buffer");
			if (vao2 != SPH_OGL_ID_NONE) {
				glDeleteVertexArrays(1, &vao2);
			}
			glGenVertexArrays(1, &vao2);
			SPH_ASSERT(vao2 != SPH_OGL_ID_NONE, "OpenGL error: Could not generate secondary VAO");

			glBindVertexArray(vao2);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			SPH_OGL_SET_VAO_ATTRIBUTE(pos, 0, GL_FLOAT, 2);
			SPH_OGL_SET_VAO_ATTRIBUTE(tex, 1, GL_FLOAT, 2);
			SPH_OGL_SET_VAO_ATTRIBUTE(col, 2, GL_FLOAT, 4);
			SPH_OGL_SET_VAO_INT_ATTRIBUTE(slot, 3, GL_BYTE, 1);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

			glBindVertexArray(SPH_OGL_ID_NONE);
		} else {
			SPH_LOG("Switching back to Primary Dynamic Vertex Buffer");
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

			glBindVertexArray(SPH_OGL_ID_NONE);
		}
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