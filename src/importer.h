#pragma once
#ifndef ASSIMP_CLASS_H
#define ASSIMP_CLASS_H

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


#define ASSIMP_FLAGS ( \
    aiProcess_Triangulate | \
	aiProcess_GenSmoothNormals | \
	aiProcess_JoinIdenticalVertices)

class MeshImporter {
	public:
		MeshImporter(const std::string& model_path, const std::string& material_path);
		
		void buffers()
		void free();

		std::vector<glm::vec2> m_uvs;
		std::vector<Texture*> m_Textures;
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec3> m_vertices, m_normals;

		std::vector<unsigned int> m_numIndices;
		std::vector<unsigned int> m_materialIndex;

		std::vector<VAO*> m_VAO;
		std::vector<VBO*> m_VBO;
		std::vector<VBO*> m_VBOUVs;
		std::vector<VBO*> m_VBONormal;
		std::vector<EBO*> m_EBO;
};

#endif // !ASSIMP_CLASS_H

