#include "OBJLoader.h"
#include <fstream>
#include <string>


void OBJLoader::load(const char* filePath) {
	std::ifstream file(filePath);
	std::string line;

	
	// Size determine
	while (std::getline(file, line)) {
		if (line[0] == 'v' && line[1] == ' ') {
			vertexCount++;
		}

		if (line[0] == 'f' && line[1] == ' ') {
			faceCount++;
		}
	}


	// Memory initialization
	vertices = new float[vertexCount * 8];
	indices = new unsigned int[faceCount * 3];

	file.close();

	float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;

	// Reading
	file.open(filePath);
	int vertexIndex = 0;
	int faceIndex = 0;
	while (std::getline(file, line)) {
		if (line[0] == 'v') {
			int v1Start = 2;
			int v2Start = line.find(' ', v1Start + 1) + 1;
			int v3Start = line.find(' ', v2Start + 1) + 1;
			int v4Start = line.find(' ', v3Start + 1) + 1;
			int v5Start = line.find(' ', v4Start + 1) + 1;
			int v6Start = line.find(' ', v5Start + 1) + 1;
			int v7Start = line.find(' ', v6Start + 1) + 1;
			int v8Start = line.find(' ', v7Start + 1) + 1;
			

			float x = std::stof(line.substr(v1Start, v2Start - v1Start - 1));
			float y = std::stof(line.substr(v2Start, v3Start - v2Start - 1));
			//float z = std::stof(line.substr(v3Start, -1));
			float z = std::stof(line.substr(v3Start, v4Start - v3Start - 1));
			float vnx = std::stof(line.substr(v4Start, v5Start - v4Start - 1));
			float vny = std::stof(line.substr(v5Start, v6Start - v5Start - 1));
			float vnz = std::stof(line.substr(v6Start, v7Start - v6Start - 1));

			float uvX = std::stof(line.substr(v7Start, v8Start - v7Start - 1));
			float uvY = std::stof(line.substr(v8Start, -1));

			//printf_s("Vertex: <%f>  <%f>  <%f>\n", x, y, z);
			sumX += x;
			sumY += y;
			sumZ += z;

			vertices[vertexIndex] = x;
			vertices[vertexIndex + 1] = y;
			vertices[vertexIndex + 2] = z;
			vertices[vertexIndex + 3] = vnx;
			vertices[vertexIndex + 4] = vny;
			vertices[vertexIndex + 5] = vnz;
			vertices[vertexIndex + 6] = uvX;
			vertices[vertexIndex + 7] = uvY;
			vertexIndex += 8;
		}

		if (line[0] == 'f') {
			int v1Start = 2;
			int v2Start = line.find(' ', v1Start + 1) + 1;
			int v3Start = line.find(' ', v2Start + 1) + 1;


			unsigned int f1 = std::stoul(line.substr(v1Start, v2Start - v1Start - 1));
			unsigned int f2 = std::stoul(line.substr(v2Start, v3Start - v2Start - 1));
			unsigned int f3 = std::stoul(line.substr(v3Start, 11));

			//printf_s("Face: <%d>  <%d>  <%d>\n", f1, f2, f3);

			indices[faceIndex] = f1 - 0;
			indices[faceIndex + 1] = f2 - 0;
			indices[faceIndex + 2] = f3 - 0;
			faceIndex += 3;
		}
	}

	cx = sumX / vertexCount;
	cy= sumY / vertexCount;
	cz = sumZ / vertexCount;
}

void OBJLoader::print() {
	
}
