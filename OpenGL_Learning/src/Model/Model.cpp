#include "Model.h"

namespace Model {
	Model::Model(const char* path) {
		loadModel(path);
	}

	void Model::load(const char* path) {
		loadModel(path);
	}

	void Model::draw(GLProgram& shader) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].draw(shader);
		}
	}

	void Model::clear() {
		meshes.clear();
		directory.clear();
		textures_loaded.clear();
		directory.clear();
	}

	void Model::setTextureFilter(int filter) {
		if (filter == GL_LINEAR) {
			linearFilter = true;
			nearestFilter = false;
		}
		if (filter == GL_NEAREST) {
			nearestFilter = true;
			linearFilter = false;
		}
	}

	void Model::loadModel(string path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene) {
		// Process all node's meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// Do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex>       vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;

		// Vertices processing
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			if (mesh->HasNormals()) {
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}

			if (mesh->mTextureCoords[0]) { // Is there a texture?
				vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			} else {
				vertex.texCoords = vec2(0);
			}

			vertices.push_back(vertex);
		}

		// Indices processing
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Texture processing
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "emissive");
			textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;

			for (unsigned int j = 0; j < textures_loaded.size(); j++) {
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {   // if texture hasn't been loaded already, load it
				Texture tex;

				tex.ID = TextureFromFile(str.C_Str(), directory);
				tex.type = typeName;
				tex.path = str.C_Str();
				textures.push_back(tex);
				textures_loaded.push_back(tex); // add to loaded textures
			}
		}

		return textures;
	}

	unsigned int Model::TextureFromFile(const char* path, const string& directory) {
		string filename = string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data) {
			unsigned int format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (linearFilter) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			if (nearestFilter) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			stbi_image_free(data);
		} else {
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}
