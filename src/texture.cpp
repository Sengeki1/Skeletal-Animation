#include "texture.h"

Texture::Texture(
	int unit, 
	const char* imgPath
) 
{
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	int width, height, nrChannels;
	GLenum format;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);

	if (nrChannels == 4) {
		format = GL_RGBA;
	}
	else {
		format = GL_RGB;
	}

	if (!data) {
		error = true;
		std::cout << "Failed to load texture " << stbi_failure_reason() << " " << imgPath << std::endl;
		return;
	} 
	else {
		printf("Width: %d, Height: %d, Channels: %d\n", width, height, nrChannels);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	error = false;
}

void Texture::Bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
