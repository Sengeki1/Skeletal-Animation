#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}

void VAO::Linking(
	VBO* vbo,
	const GLuint& layout, 
	const GLuint& size, 
	const GLenum& type, 
	const GLsizeiptr& stride, 
	void* offset
) {
	glBindVertexArray(ID);
	vbo->Bind();
	if (type != GL_INT) {
		glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	}
	else {
		glVertexAttribIPointer(layout, size, type, stride, offset);
	}
	glEnableVertexAttribArray(layout);
	vbo->Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

VAO::~VAO() 
{
	glDeleteVertexArrays(1, &ID);
}

