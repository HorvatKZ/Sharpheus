#include "pch.h"
#include "OGLShader.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"

namespace Sharpheus::OpenGL {

	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
		: vertexShader(vertexShader), fragmentShader(fragmentShader)
	{
	}


	ShaderProgram::~ShaderProgram()
	{
		if (used) {
			Unuse();
		}

		if (programID != SPH_OGL_ID_NONE) {
			glDeleteProgram(programID);
		}
	}


	void ShaderProgram::Init()
	{
		AttachShaders();
		BindAttributes();
		LinkProgram();
		inited = true;

		glUseProgram(programID);
		GLint loc = GetUniformLoc("Textures");
		GLint* slots = new GLint[SPH_OGL_MAX_SUPPORTED_TEXTURE_SLOTS];
		for (uint32 i = 0; i < SPH_OGL_MAX_SUPPORTED_TEXTURE_SLOTS; ++i) {
			slots[i] = i;
		}
		glUniform1iv(loc, 1, slots);
		delete[] slots;
	}


	void ShaderProgram::AttachShaders()
	{
		SPH_LOG("Creating shader program from vertex shader: \"{0}\" and fragmment shader \"{1}\"", vertexShader, fragmentShader);
		GLuint vs_ID = LoadShader(GL_VERTEX_SHADER, vertexShader.c_str());
		GLuint fs_ID = LoadShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());

		programID = glCreateProgram();
		if (programID == SPH_OGL_ID_NONE) {
			GLenum err = glGetError();
			SPH_ERROR("OpenGL Error: {0} - {1}. Could not create shader program", err, gluErrorString(err));
		}

		glAttachShader(programID, vs_ID);
		glAttachShader(programID, fs_ID);

		glDeleteShader(vs_ID);
		glDeleteShader(fs_ID);
	}


	void ShaderProgram::BindAttributes()
	{
		glBindAttribLocation(programID, 0, "vs_in_pos");
		glBindAttribLocation(programID, 1, "vs_in_tex");
		glBindAttribLocation(programID, 2, "vs_in_col");
		glBindAttribLocation(programID, 3, "vs_in_slot");
	}


	void ShaderProgram::LinkProgram()
	{
		glLinkProgram(programID);

		GetAndLogStatus(programID, "Shader program successfully linked;", "Error in linking shader program;", GL_LINK_STATUS);
	}


	GLuint ShaderProgram::LoadShader(GLenum type, const std::string& fname)
	{
		GLuint loadedShader = glCreateShader(type);

		if (loadedShader == SPH_OGL_ID_NONE) {
			SPH_ERROR("Error in creating \"{0}\" shader (glCreateShader)!", fname);
			return SPH_OGL_ID_NONE;
		}

		FILE* file = fopen(ResourceManager::GetShaderPath(fname).c_str(), "rb");
		if (file == NULL) {
			SPH_ERROR("Cannot open shader \"{0}\" to load", fname);
			return SPH_OGL_ID_NONE;
		}

		fseek(file, 0L, SEEK_END);
		long fileSize = ftell(file);
		rewind(file);

		char* content = new char[fileSize + 1];

		if (fread(content, fileSize, 1, file) != 1) {
			SPH_ERROR("Cannot read content from shader \"{0}\"", fname);
			fclose(file);
			delete[] content;
			return SPH_OGL_ID_NONE;
		}
		fclose(file);
		content[fileSize] = '\0';

		glShaderSource(loadedShader, 1, &content, NULL);
		glCompileShader(loadedShader);

		delete[] content;

		if (!GetAndLogStatus(loadedShader, "Shader \"" + fname + "\" successfully compiled;", "Error in compiling shader \"" + fname + "\";", GL_COMPILE_STATUS)) {
			glDeleteShader(loadedShader);
			return SPH_OGL_ID_NONE;
		}

		return loadedShader;
	}


	bool ShaderProgram::GetAndLogStatus(GLuint programID, const std::string& successText, const std::string& errorText, GLuint status)
	{
		GLint infoLogLength = 0, result = GL_FALSE;
		switch (status) {
			case GL_COMPILE_STATUS:
				glGetShaderiv(programID, status, &result);
				glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
				break;
			case GL_LINK_STATUS:
				glGetProgramiv(programID, status, &result);
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
				break;
		}

		if (result == GL_FALSE || infoLogLength != 0) {
			char* errorMsg = new char[infoLogLength + 1];
			switch (status) {
				case GL_COMPILE_STATUS:
					glGetShaderInfoLog(programID, infoLogLength, NULL, errorMsg);
					break;
				case GL_LINK_STATUS:
					glGetProgramInfoLog(programID, infoLogLength, NULL, errorMsg);
					break;
			}
			errorMsg[infoLogLength] = '\0';

			SPH_ERROR(errorText + " ID: {0}; ERROR: {1}", programID, errorMsg);
			delete[] errorMsg;
			return false;
		}

		SPH_INFO(successText + " ID: {0}", programID);
		return true;
	}

}