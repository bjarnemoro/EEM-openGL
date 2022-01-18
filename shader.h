#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	void use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void Setu4(const std::string& name, float v1, float v2, float v3, float v4) const;

	void SetMat4(const std::string& name, float* value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float v1, float v2, float v3) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetVec4(const std::string& name, float v1, float v2, float v3, float v4) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;
};