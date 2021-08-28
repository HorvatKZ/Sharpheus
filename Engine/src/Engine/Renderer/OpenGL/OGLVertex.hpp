#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#define OGL_ID_NONE 0


namespace Sharpheus::OpenGL {

	struct Vertex {
		glm::vec2 pos, tex;
		glm::vec4 col;
		GLuint slot;

		Vertex() {}
		Vertex(const glm::vec2& pos, const glm::vec2& tex, const glm::vec4 col, GLuint slot = 0)
			: pos(pos), tex(tex), col(col), slot(slot) {}
	};

}