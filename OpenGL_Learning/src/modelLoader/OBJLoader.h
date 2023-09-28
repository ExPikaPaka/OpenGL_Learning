#pragma once

#include <vector>
class OBJLoader {
public:
	float* vertices;
	unsigned int* indices;


	unsigned int vertexCount = 0;
	unsigned int faceCount = 0;
	void load(const char* filePath);
	void print();

	float cx, cy, cz;
};

