#ifndef VAO_IMPLEMENTATIO_H
#define VAO_IMPLEMENTATIO_H

#include <iostream>
#include <glad/glad.h>
#include "VBO.h"

class VBO;

class VAO
{
	public:
		VAO();
		~VAO();
		void Bind();
		void Linking(
			VBO* vbo,
			const GLuint& layout, 
			const GLuint& size, 
			const GLenum& type, 
			const GLsizeiptr& stride, 
			void* offset
		);
		void Unbind();
	private:
		GLuint ID = 0;
};

#endif
