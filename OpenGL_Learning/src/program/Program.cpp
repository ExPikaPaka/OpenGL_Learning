#include "Program.h"

bool Program::init(const char* windowTitle, int width, int height) {
	// Logger message initialization 
	err.init("[ERROR]", Color(242, 242, 242), Color(197, 15, 31));
	info.init("[INFO]", Color(242, 242, 242), Color(58, 150, 221));
	logger.logToConsole(true);

	// Initializing GLFW
	if (glfwInit()) {
		logger.addCombined(info, " GLFW Initialized successfully.", "\n");
	} else {
		logger.addCombined(err, " Failed to initialize GLFW.", "\n");
		return false;
	}

	// Setting GLFW to OpenGL 3.3 version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating window
	window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (window) {
		logger.addCombined(info, " GLFW window created successfully.", "\n");
	} else {
		logger.addCombined(err, " Failed to create GLFW window.", "\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);



	// Loading GLAD
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger.addCombined(info, " GLAD initialized successfully.", "\n");
	} else {
		logger.addCombined(err, " Failed to initialize GLAD.", "\n");
		return false;
	}

	// Setting OpenGL window size
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return true;
}

void Program::update() {
	glfwPollEvents();
	processInput();
}

void Program::render() {
	glClearColor(ctof(20, 20, 20), 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);
}

void Program::clearBuffers() {
	glClearColor(ctof(20, 20, 20), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Program::swapBuffers() {
	glfwSwapBuffers(window);
}

void Program::terminate() {
	glfwTerminate();
}

bool Program::shouldClose() {
	return glfwWindowShouldClose(window);
}

GLFWwindow* Program::getWidow() {
	return window;
}

void Program::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Program::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
