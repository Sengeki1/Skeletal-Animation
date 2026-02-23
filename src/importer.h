#pragma once
#ifndef ASSIMP_CLASS_H
#define ASSIMP_CLASS_H

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "texture.h"
#include "common.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


#define ASSIMP_FLAGS ( \
    aiProcess_Triangulate | \
	aiProcess_GenSmoothNormals | \
	aiProcess_JoinIdenticalVertices | \
	aiProcess_LimitBoneWeights)

class MeshImporter {
	public:
		MeshImporter(const std::string& model_path, const std::string& material_path);

		std::vector<glm::vec2> m_uvs;
		std::vector<glm::vec4> m_weights;
		std::vector<glm::ivec4> m_boneIDs;
		std::vector<Texture*> m_Textures;
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec3> m_vertices, m_normals;

		std::vector<unsigned int> m_numIndices;
		std::vector<unsigned int> m_materialIndex;

		std::vector<VAO*> m_VAO;
		std::vector<VBO*> m_VBO;
		std::vector<VBO*> m_VBOUVs;
		std::vector<VBO*> m_VBONormal;
		std::vector<VBO*> m_VBOWeights;
		std::vector<VBO*> m_VBOBonesIDs;
		std::vector<EBO*> m_EBO;

		// skeletal info
		aiNode* rootNode;
		aiAnimation* animation;

		std::map<std::string, int> bonesMapping; // a dict of all the bones with their index
		std::map<std::string, glm::mat4> bonesOffsetMatrix; // a dict of all the offsets based on their bones
		std::map<int, std::pair<std::vector<float>, std::vector<int>>> verticesBoneInfo; // all the vertices affected by bones and their weights
		std::vector<glm::mat4> finalBoneMatrix;

	private:
		void buffers(const int& index, const aiMesh* pMesh);
		void free();

		Assimp::Importer importer;
		int boneIndex;
};

#endif // !ASSIMP_CLASS_H

