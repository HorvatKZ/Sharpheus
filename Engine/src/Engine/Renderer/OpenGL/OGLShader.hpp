#pragma once

#include "pch.h"
#include "OGLVertex.hpp"

#define SPH_OGL_SETUNIFORM(type, glFunction, ...) \
	inline void SetUniform(const std::string& name, type value) { \
		if (!used) { \
			Use(); \
		} \
		glFunction(GetUniformLoc(name), __VA_ARGS__); \
	}


namespace Sharpheus::OpenGL {
	
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		virtual ~ShaderProgram();

		void Init();

		inline void Use() {
			if (!inited) {
				Init();
			}
			glUseProgram(programID);
			used = true;
		}

		inline void Unuse() {
			glUseProgram(SPH_OGL_ID_NONE);
			used = false;
		}

		SPH_OGL_SETUNIFORM(float, glUniform1f, value)
		SPH_OGL_SETUNIFORM(const glm::vec2&, glUniform2f, value.x, value.y)
		SPH_OGL_SETUNIFORM(const glm::vec3&, glUniform3f, value.x, value.y, value.z)
		SPH_OGL_SETUNIFORM(const glm::vec4&, glUniform4f, value.x, value.y, value.y, value.w)
		SPH_OGL_SETUNIFORM(const glm::mat4&, glUniformMatrix4fv, 1, GL_FALSE, &(value[0][0]))
		inline void SetUniformPoint(const std::string& name, const Point& value) { SetUniform(name, value.ToVec2()); }
		inline void SetUniformColor3(const std::string& name, const Color& value) { SetUniform(name, value.ToVec3()); }
		inline void SetUniformColor4(const std::string& name, const Color& value) { SetUniform(name, value.ToVec4()); }

	private:
		GLuint programID = 0;
		std::unordered_map<std::string, GLuint> uniforms;
		std::string vertexShader, fragmentShader;
		bool inited = false, used = false;

		void	AttachShaders();
		void	BindAttributes();
		void	LinkProgram();
		GLuint	LoadShader(GLenum type, const std::string& fname);

		inline GLuint GetUniformLoc(const std::string& name) {
			auto it = uniforms.find(name);
			if (it != uniforms.end()) {
				return it->second;
			}
			
			GLuint loc = glGetUniformLocation(programID, name.c_str());
			uniforms.insert({ name, loc });
			return loc;
		}

		static bool	GetAndLogStatus(GLuint programID, const std::string& successText, const std::string& errorText, GLuint status);
	};

}