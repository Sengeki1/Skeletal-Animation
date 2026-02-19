#ifndef SHADER_IMPLEMENTATION_H
#define SHADER_IMPLEMENTATION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cerrno>
#include <string>
#include <glad/glad.h>

class Shader 
{
	public:
		GLuint ID;

		explicit Shader(
			const char* vert, 
			const char* frag
		) :
			m_vert(vert), 
			m_frag(frag),
			m_vertexShader(0),
			m_fragmentShader(0),
			ID(0)
		{};
		~Shader() = default;

		void Create();
		void Activate();
		void Deactivate();
	private:
		const char* m_vert;
		const char* m_frag;
		GLuint m_vertexShader;
		GLuint m_fragmentShader;
};

#endif // !SHADER_IMPLEMENTATION_H

