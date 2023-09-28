#include "Button.h"
#include <iostream>


void Button::init(GLFWwindow* window, unsigned int gl_Program) {
	this->window = window;
	this->gl_Program = gl_Program;

	selectionUniform = glGetUniformLocation(gl_Program, "selected");
}

void Button::setShape(int x, int y, int w, int h) {
	int windowWidth = 0;
	int windowHeight = 0;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	float x1 = 2.0 * (float)x / (float)windowWidth - 1.0;
	float y1 = 1.0 - 2.0 * (float)y / (float)windowHeight;
	float x2 = x1 + 2.0 * (float)w / (float)windowWidth;
	float y2 = y1 - 2.0 * (float)h / (float)windowHeight;


	// Vertices
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = 0;

	vertices[3] = r;
	vertices[4] = g;
	vertices[5] = b;

	vertices[6] = 0.0f;
	vertices[7] = 1.0f;



	vertices[8] = x2;
	vertices[9] = y1;
	vertices[10] = 0;

	vertices[11] = r;
	vertices[12] = g;
	vertices[13] = b;

	vertices[14] = 1.0f;
	vertices[15] = 1.0f;



	vertices[16] = x2;
	vertices[17] = y2;
	vertices[18] = 0;

	vertices[19] = r;
	vertices[20] = g;
	vertices[21] = b;

	vertices[22] = 1.0f;
	vertices[23] = 0.0f;



	vertices[24] = x1;
	vertices[25] = y2;
	vertices[26] = 0;

	vertices[27] = r;
	vertices[28] = g;
	vertices[29] = b;

	vertices[30] = 0.0f;
	vertices[31] = 0.0f;

	// Indices
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;


	for (int i = 0; i < 32; i += 8) {
		std::cout << "XYZ{" << vertices[i + 0] << " " <<  vertices[i + 1] << " " << vertices[i + 2] << "}   RGB{" << vertices[i + 3] << " " << vertices[i + 4] << " " << vertices[i + 5] << "}\n";
	}

	// Generating buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	// Binding buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Атрибут позиции (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Button::setColor(int r, int g, int b) {
	this->r = (float)r / 255.0;
	this->g = (float)g / 255.0;
	this->b = (float)b / 255.0;
}

void Button::setTexture(unsigned char* tex, int width, int height, int nChanels) {
	// Texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Loading data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (nChanels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	}
	if (nChanels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
	}
	//glGenerateMipmap(GL_TEXTURE_2D);
}

void Button::update() {
	activated = true;

	if (isActive) {
		if (selected()) {
			activated = false;
		}
	}
}

void Button::draw() {
	glUniform1i(selectionUniform, 1);
	if (activated) {
		glUniform1i(selectionUniform, 0);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Button::setState(bool state) {
	isActive = state;
}

bool Button::selected() {
	double m_x = 0, m_y = 0;

	// Check if m_bt1 is bressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		// Check intersection
		glfwGetCursorPos(window, &m_x, &m_y);
		if (m_x >= x && m_x < x + w && m_y >= y && m_y < y + h) {
			return true;
		}
	}
	return false;
}