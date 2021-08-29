#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#define SPH_OGL_ID_NONE 0
#define SPH_OGL_MAX_SUPPORTED_TEXTURE_SLOTS 32
#define SPH_OGL_MAX_QUADS_IN_BATCH 65536


namespace Sharpheus::OpenGL {

	struct Vertex {
		glm::vec2 pos, tex;
		glm::vec4 col;
		byte slot;

		Vertex() {}
		Vertex(const glm::vec2& pos, const glm::vec2& tex, const glm::vec4 col, GLuint slot = 0)
			: pos(pos), tex(tex), col(col), slot(slot) {}
	};

}