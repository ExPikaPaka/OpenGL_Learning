#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <iostream>


// Model namespace
namespace Model {
	// Simplifications
	using std::vector;
	using std::string;
	using glm::vec3;
	using glm::vec2;

	// Model class, that consists of 1+ meshes
	class Model {
	public:
		Model(const char* path);
		Model() {};
		void load(const char* path);
		void draw(GLProgram& shader);
		void clear();

		void setTextureFilter(int filter);

	private:
		vector<Texture> textures_loaded;
		vector<Mesh> meshes;
		string directory;

		bool linearFilter = true;
		bool nearestFilter = false;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
		unsigned int TextureFromFile(const char* path, const string& directory);
	};
}

