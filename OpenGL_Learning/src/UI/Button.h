#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "..\imageLoader\stb_image.h"

class Button {
	GLFWwindow* window;
	unsigned int VBO, VAO, EBO, texture;
	float vertices[32]; // x y z, r g b, u v    - x4
	unsigned int indices[6];

	unsigned int gl_Program;
	unsigned int selectionUniform;
	
	bool isActive = 1;
	bool activated = 0;

	int x, y, w, h;
	float r, g, b;
public:
	void init(GLFWwindow* window, unsigned int gl_Program);
	void setShape(int x, int y, int w, int h);
	void setColor(int r, int g, int b);
	void setTexture(unsigned char* tex, int width, int height, int nChanels);
	void update();
	void draw();

	void setState(bool state);
	bool selected();
};

