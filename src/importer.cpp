#include "importer.h"

MeshImporter::MeshImporter(const std::string &model_path, const std::string &material_path) {
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(model_path, ASSIMP_FLAGS);
	if (scene) {
		m_Textures.resize(scene->mNumMaterials);

		for (unsigned int i0 = 0; i0 < scene->mNumMeshes; i0++) {
			const aiMesh *pMesh = scene->mMeshes[i0];

			free();
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
			for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
				const aiVector3D *pPos = &(pMesh->mVertices[i]);
				const aiVector3D *pNormal = pMesh->HasNormals() ? &(pMesh->mNormals[i]) : &Zero3D;

				int uvChannel = pMesh->HasTextureCoords(0) ? 0 : pMesh->HasTextureCoords(1) ? 1
																							: -1;
				const aiVector3D *pTexCoord = pMesh->HasTextureCoords(uvChannel) ? &(pMesh->mTextureCoords[uvChannel][i]) : &Zero3D;
				float u = glm::clamp(pTexCoord->x, 0.0f, 1.0f);
				float v = glm::clamp(pTexCoord->y, 0.0f, 1.0f);

				m_uvs.push_back(glm::vec2(u, v));
				m_vertices.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
				m_normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
			}

			for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
				const aiFace &face = pMesh->mFaces[i];

				if (face.mNumIndices == 3) {
					m_indices.push_back(face.mIndices[0]);
					m_indices.push_back(face.mIndices[1]);
					m_indices.push_back(face.mIndices[2]);
				}
				else {
					std::cerr << "Warning: Non-triangular face detected!" << std::endl;
				}
			}
			buffers();
		}

		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial *pMaterial = scene->mMaterials[i];

			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString path;

				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					std::string path_s = path.data;
					std::string full_path;

					size_t index = path_s.find("\\");
					path_s = path_s.substr(index + 1, path_s.length());
					full_path = material_path + "/" + path_s;

					m_Textures[i] = new Texture(0, full_path.c_str());
					if (m_Textures[i]->error) {
						delete m_Textures[i];
						m_Textures[i] = nullptr;
					}
				}
			}
		}
	}
	else {
		std::cout << "Error parsing " << model_path << " " << importer.GetErrorString() << std::endl;
	}
}

void MeshImporter::buffers() {
	this->m_VAO.push_back(new VAO());
	this->m_VBO.push_back(new VBO(m_vertices));
	this->m_VBOUVs.push_back(new VBO(m_uvs));
	this->m_VBONormal.push_back(new VBO(m_normals));
	this->m_EBO.push_back(new EBO(m_indices));

	this->m_numIndices.push_back(m_indices.size());
	this->m_materialIndex.push_back(pMesh->mMaterialIndex);

	this->m_VAO[i0]->Linking(m_VBO[i0], 0, 3, GL_FLOAT, sizeof(glm::vec3), (void *)0);
	this->m_VAO[i0]->Linking(m_VBOUVs[i0], 1, 2, GL_FLOAT, sizeof(glm::vec2), (void *)0);
	this->m_VAO[i0]->Linking(m_VBONormal[i0], 2, 3, GL_FLOAT, sizeof(glm::vec3), (void *)0);

	this->m_VAO[i0]->Unbind();
	this->m_VBO[i0]->Unbind();
}

void MeshImporter::free() {
	m_uvs.clear();
	m_normals.clear();
	m_indices.clear();
	m_vertices.clear();

	m_uvs.resize(0);
	m_normals.resize(0);
	m_indices.resize(0);
	m_vertices.resize(0);
}