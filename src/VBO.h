#pragma once
#ifndef VBO_IMPLEMENTATION_H
#define VBO_IMPLEMENTATION_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>

class VBO
{
	public:
		VBO() :
			ID(0)
		{};
		VBO(
			const std::vector<glm::vec3>& vertices
		);
		VBO(
			const std::vector<glm::vec2>& vertices
		);
		VBO(
			const std::vector<glm::vec4>& vertices
		);
		VBO(
			const std::vector<glm::ivec4>& vertices
		);
		~VBO();
		void Bind();
		void Unbind();
	private:
		GLuint ID = 0;
};

#endif // VBO_IMPLEMENTATION_H
