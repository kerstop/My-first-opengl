#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader 
{
public:

	unsigned int ID;

	// construct a shader
	Shader(const char* vertexPath, const char* fragmentPath) {
	
		std::string vertexSource;
		std::string fragmentSource;
		std::ifstream vertexSourceFile;
		std::ifstream fragmentSourceFile;

		vertexSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		fragmentSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		try {
			vertexSourceFile.open(vertexPath);
			fragmentSourceFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vertexSourceFile.rdbuf();
			fShaderStream << fragmentSourceFile.rdbuf();		

			vertexSourceFile.close();
			fragmentSourceFile.close();

			vertexSource   = vShaderStream.str();
			fragmentSource = fShaderStream.str();	

		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* c_vertexSource = vertexSource.c_str();
		const char* c_fragmentSource = fragmentSource.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &c_vertexSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &c_fragmentSource, NULL);
		glCompileShader(fragmentShader);
		
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;	
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;	
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	// set as active shader
	void use()
	{
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

};

