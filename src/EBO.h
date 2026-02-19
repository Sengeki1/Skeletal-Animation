#pragma once
#ifndef EBO_IMPLEMENTATION_H
#define EBO_IMPLEMENTATION_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>

class EBO
{
	public:
		EBO() :
			ID(0)
		{};
		EBO(
			const std::vector<int>& indices
		);
		EBO(
			const std::vector<unsigned int>& indices
		);
		~EBO();
		void Unbind();
	private:
		GLuint ID = 0;
};

#endif // EBO_IMPLEMENTATION_H