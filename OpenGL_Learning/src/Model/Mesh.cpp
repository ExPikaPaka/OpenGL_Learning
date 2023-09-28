#include "Mesh.h"

Model::Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Model::Mesh::draw(GLProgram& shader) {
	// Apply textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int emissiveNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // Activating texture before binding

		string number;
		string type = textures[i].type;
		if (type == "diffuse")
			number = std::to_string(diffuseNr++);
		if (type == "specular")
			number = std::to_string(specularNr++);
		if (type == "emissive")
			number = std::to_string(emissiveNr++);

		shader.setInt(("material." + type + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}

	// Render
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Model::Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Position bind
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	// Normal bind
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// TexCoords bind
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}
