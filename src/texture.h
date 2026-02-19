#pragma once
#ifndef TEXTURE_IMPLEMENTATION_HEADER
#define TEXTURE_IMPLEMENTATION_HEADER

#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>

class Texture
{
	public:
		Texture(
			int unit,
			const char* imgPath
		);
		~Texture() = default;
		void Bind(int unit);
		void Unbind();
		void Delete();

		bool error;
	private:
		GLuint ID;
};

#endif // !TEXTURE_IMPLEMENTATION_HEADER

