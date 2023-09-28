#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../logSystem/Logger.h"

class Program {
	GLFWwindow* window;
	const char* windowTitle;

	Logger logger;
	Message err;
	Message info;

public:
	int width;
	int height;

	bool init(const char* windowTitle, int width, int height);
	void update();
	void render();

	void clearBuffers();
	void swapBuffers();
	void terminate();
	bool shouldClose();

	GLFWwindow* getWidow();


private:
	void processInput();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};




// Converts 0-255 color to float 0-1 values like "ctof(255, 0, 51)" to "1.0, 0.0, 0.2" 
#define ctof(r, g, b) (float)r / 255, (float)g / 255, (float)b/ 255


/*! \brief Convert HSV to RGB color space

  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.

  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]

*/
void HSVtoRGB(float fH, float fS, float fV, float& fR, float& fG, float& fB);
