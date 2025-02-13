#ifndef SHADER_JP
#define SHADER_JP

#include <iostream>

// openGL function loader
#include <glad/glad.h>

// matrices & matrix manipulation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// file loading & string libraries
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ID;
	// create new shader program
	void init(const char* vertexPath, const char* fragmentPath);

	// activate the shader
	void use();

	// utility uniform functions
	// -------------------------
	// "const" after the function means that the function is part of a class and cant change any members of that class.
	void setBool(const std::string& name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, glm::vec3 value) const;
	// ------------------------------------------------------------------------
	void deleteMyself();

private:
	// utility function for checking shader compilation/linking errors.
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif