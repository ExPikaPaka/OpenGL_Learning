#pragma once
#include "../shaderLoader/GLProgram.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

// Model namespace
namespace Model {
	// Simplifications
	using std::vector;
	using std::string;
	using glm::vec3;
	using glm::vec2;

	// Single vertex structure
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texCoords;
	};

	// Texture structure
	struct Texture {
		unsigned int ID;
		string type;
		string path;
	};

	// Single Mesh class
	class Mesh {
	public:
		vector<Vertex>       vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void draw(GLProgram& shader);

	private:
		unsigned int VAO, VBO, EBO;
		void setupMesh();
	};
}
