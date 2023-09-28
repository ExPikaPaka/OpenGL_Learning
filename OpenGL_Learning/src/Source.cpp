//#define NOMINMAX
#include "program/Program.h"
#include "shaderLoader/GLProgram.h"
#include "modelLoader/OBJLoader.h"
#include "camera/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Model/Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "imageLoader/stb_image.h"

using namespace glm;


// ------------ Camera parameters ------------
// Updates yaw & pitch whenever mouse moved
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void keyboardUpdate(GLFWwindow* window);
Camera camera;


struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float emissivity;
};

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Light type
	// 0 - point (no attenuation)
	// 1 - point (with attenuation)
	// 2 - directional
	// 3 - spotlight (no attenuation)
	// 4 - spotlight (with attenuation)
	int type;

	// Point light parameters
	float constant;
	float linear;
	float quadratic;

	// Spotlight parameters
	float cutoff;
	float outerCutoff;
};




int main() {
	// ------------ Logger message initialization  ------------
	Logger logger;
	Message err;
	Message info;

	err.init("[ERROR]", Color(242, 242, 242), Color(197, 15, 31));
	info.init("[INFO]", Color(242, 242, 242), Color(58, 150, 221));
	logger.logToConsole(true);



	// ------------ Window initialization ------------
	Program game;
	game.init("Title", 1366, 768);
	glfwSetCursorPosCallback(game.getWidow(), mouseCallback);
	glfwSetScrollCallback(game.getWidow(), scrollCallback);
	glfwSetInputMode(game.getWidow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	//glfwSwapInterval(0);


	// ------------ Setting up IMGUI ------------
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 330 core";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(game.getWidow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);





	// ------------ Shaders initialization
	std::string objectVertShaderPath("src/shaders/objectTex.vert");
	std::string objectFragShaderPath("src/shaders/objectTex.frag");
	std::string lightVertShaderPath("src/shaders/light.vert");
	std::string lightFragShaderPath("src/shaders/light.frag");
	std::string skyboxFragShaderPath("src/shaders/skyboxTex.frag");

	GLProgram objectProgram;
	objectProgram.init(objectVertShaderPath, objectFragShaderPath);

	GLProgram lightProgram;
	lightProgram.init(lightVertShaderPath, lightFragShaderPath);

	GLProgram skyboxProgram;
	skyboxProgram.init(objectVertShaderPath, skyboxFragShaderPath);



	// ------------ Loading object ------------
	std::string objectPath("assets/box.obj");

	OBJLoader object;
	object.load(objectPath.c_str());



	// ------------ Creating buffers ------------
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 333 * sizeof(float), object.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 333 * sizeof(unsigned int), object.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// ------------ Defining object & light parameters ------------
	Material material;
	material.ambient = vec3(1, 1, 1.0);
	material.diffuse = vec3(1, 1, 1);
	material.specular = vec3(0.35);
	material.shininess = 3.35;
	material.emissivity = 0.5;

	Light light[4];
	light[0].ambient = vec3(0.05);
	light[0].diffuse = vec3(0.13, 0.215, 0.22);
	light[0].specular = vec3(0.33, 0.47, 0.46);
	light[0].position = vec3(0, 0, 34);
	light[0].direction = vec3(-0, -0, -1);
	light[0].type = 2;
	light[0].linear = 0;
	light[0].constant = 1;
	light[0].quadratic = 0.0005;
	light[0].cutoff = 20;
	light[0].outerCutoff = 2.5;

	light[1].ambient = vec3(0.05);
	light[1].diffuse = vec3(0.41, 0.109, 0);
	light[1].specular = vec3(1, 0.5, 0);
	light[1].position = vec3(0, 19, 5);
	light[1].direction = vec3(0, 0, -1);
	light[1].type = 1;
	light[1].linear = 0.02;
	light[1].constant = 1;
	light[1].quadratic = 0.001;
	light[1].cutoff = 30;
	light[1].outerCutoff = 5;

	light[2].ambient = vec3(0.05);
	light[2].diffuse = vec3(0.5);
	light[2].specular = vec3(1.0);
	light[2].position = vec3(4, 5, -1);
	light[2].direction = vec3(0, 0, -1);
	light[2].type = 3;
	light[2].linear = 1;
	light[2].constant = 1;
	light[2].quadratic = 1;
	light[2].cutoff = 30;
	light[2].outerCutoff = 5;

	light[3].ambient = vec3(0.05);
	light[3].diffuse = vec3(0.5);
	light[3].specular = vec3(1.0);
	light[3].position = vec3(6, 5, -1);
	light[3].direction = vec3(0, 0, -1);
	light[3].type = 3;
	light[3].linear = 1;
	light[3].constant = 1;
	light[3].quadratic = 1;
	light[3].cutoff = 30;
	light[3].outerCutoff = 5;



	int selLight = 0;
	bool flashlight = false;
	bool restrictArea = false;

	glm::vec3 modelRotation = glm::vec3(90, 0, 0);
	glm::vec3 modelScale = glm::vec3(10);
	glm::vec3 modelPosition = glm::vec3(0);
	glm::vec3 clearColor = glm::vec3(0.5);
	float multiplier = 1;

	
	// ------------ Camera parameters ------------
	glm::mat4 view;
	glm::mat4 model;
	glm::mat4 projection;
	camera.position = glm::vec3(0, 0, 2);

	

	// Test
	Model::Model world;
	world.load("assets/world/world.obj");

	Model::Model skybox("assets/skybox/skybox.obj");
	
	char path[256] = "assets/minecraft/Mineways2Skfb.obj";
	Model::Model sceneObject;
	sceneObject.setTextureFilter(GL_NEAREST);
	sceneObject.load(path);
	glEnable(GL_CULL_FACE);

	// ------------ Game loop ------------
	while (!game.shouldClose()) {
		/* ------ Update ------ */
		game.update();
		keyboardUpdate(game.getWidow());

		if (restrictArea) {
			if (camera.position.z < 0.2) camera.position.z = 0.2;
			if (camera.position.z > 60) camera.position.z = 60;
			if (camera.position.x < -19.8) camera.position.x = -19.8;
			if (camera.position.y < -19.8) camera.position.y = -19.8;
			if (camera.position.x > 19.8) camera.position.x = 19.8;
			if (camera.position.y > 19.8) camera.position.y = 19.8;
		}

		if (glfwGetKey(game.getWidow(), GLFW_KEY_C)) {
			glfwSetInputMode(game.getWidow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (glfwGetKey(game.getWidow(), GLFW_KEY_V)) {
			glfwSetInputMode(game.getWidow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		/* ------ ====== ------ */

		//camera.print();
		
		/* ------ Render ------ */
		game.clearBuffers();
		glCullFace(GL_BACK);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Rendering object
		objectProgram.use();
		objectProgram.setVec3("material.ambient", material.ambient);
		objectProgram.setVec3("material.diffuse", material.diffuse);
		objectProgram.setVec3("material.specular", material.specular);
		objectProgram.setFloat("material.shininess", material.shininess);
		objectProgram.setFloat("material.emissivity", material.emissivity);
		objectProgram.setInt("lights_count", 4);

		for (int i = 0; i < 4; i++) {
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].ambient")).c_str(), light[i].ambient);
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].diffuse")).c_str(), light[i].diffuse);
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].specular")).c_str(), light[i].specular);
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].position")).c_str(), light[i].position);
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].direction")).c_str(), light[i].direction);
			objectProgram.setFloat((std::string("light[" + std::to_string(i)) + std::string("].linear")).c_str(), light[i].linear);
			objectProgram.setFloat((std::string("light[" + std::to_string(i)) + std::string("].constant")).c_str(), light[i].constant);
			objectProgram.setFloat((std::string("light[" + std::to_string(i)) + std::string("].quadratic")).c_str(), light[i].quadratic);
			objectProgram.setInt((std::string("light[" + std::to_string(i)) + std::string("].type")).c_str(), light[i].type);
			objectProgram.setFloat((std::string("light[" + std::to_string(i)) + std::string("].cutoff")).c_str(), cos(glm::radians(light[i].cutoff)));
			objectProgram.setFloat((std::string("light[" + std::to_string(i)) + std::string("].outerCutoff")).c_str(), cos(glm::radians(light[i].outerCutoff + light[i].cutoff)));
			objectProgram.setVec3((std::string("light[" + std::to_string(i)) + std::string("].ambient")).c_str(), light[i].ambient);
		}
		objectProgram.setVec3("viewPos", camera.position);

		if (flashlight) {
			objectProgram.setVec3((std::string("light[" + std::to_string(selLight)) + std::string("].position")).c_str(), camera.position);
			objectProgram.setVec3((std::string("light[" + std::to_string(selLight)) + std::string("].direction")).c_str(), camera.front);
		}
		


		// Render object
		model = glm::mat4(1);
		model = glm::scale(model, modelScale);
		model = glm::translate(model, modelPosition);
		model = glm::rotate(model, glm::radians(modelRotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(modelRotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(modelRotation.z), glm::vec3(0, 0, 1));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), 16.0f / 9.0f, 0.1f, 1000000.0f);
		
		objectProgram.setMat4("model", model);
		objectProgram.setMat4("view", view);
		objectProgram.setMat4("projection", projection);
		sceneObject.draw(objectProgram);
		
	 
	
		// Rendering world
		objectProgram.use();
		model = glm::mat4(1);
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), 16.0f / 9.0f, 0.1f, 1000000.0f);
		objectProgram.setMat4("model", model);
		objectProgram.setMat4("view", view);
		objectProgram.setMat4("projection", projection);
		world.draw(objectProgram);



		// Render skybox
		skyboxProgram.use();
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1000));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), 16.0f / 9.0f, 0.1f, 1000000.0f);
		skyboxProgram.setMat4("model", model);
		skyboxProgram.setMat4("view", view);
		skyboxProgram.setMat4("projection", projection);
		skybox.draw(skyboxProgram);



		// Rendering light
		lightProgram.use();
		model = glm::mat4(1);
		projection = glm::perspective(glm::radians(camera.fov), 1366.0f / 768.0f, 0.1f, 100000.0f);
		view = camera.getViewMatrix();

		for (int i = 0; i < 4; i++) {
			model = glm::translate(glm::mat4(1), light[i].position);
			model = glm::scale(model, glm::vec3(0.5));

			lightProgram.setMat4("model", model);
			lightProgram.setMat4("view", view);
			lightProgram.setMat4("projection", projection);

			if (flashlight && i == selLight) continue;
			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/* ------ ====== ------ */



		/* ------ INGUI Render ------ */
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Creating window
		int flags = 0;
		static float min = -10;
		static float max = 10;
		static float mult = 1;
		static bool wireframeMode = false;

		ImGui::SetNextWindowBgAlpha(0.4);
		ImGui::Begin("Scene edit");

		ImGui::Text("Material parameters:");               
		ImGui::ColorEdit3("Material.ambient", &material.ambient.r, flags);
		ImGui::ColorEdit3("Material.diffuse", &material.diffuse.r, flags);
		ImGui::ColorEdit3("Material.specular", &material.specular.r, flags);
		ImGui::SliderFloat("Material.shininess", &material.shininess, 0.1, 128);
		ImGui::SliderFloat("Material.emissivity", &material.emissivity, 0, 5);
		
		ImGui::Text("Light parameters:");
		ImGui::InputInt("Light number", &selLight);
		ImGui::ColorEdit3("Light.ambient", &light[selLight].ambient.r, flags);
		ImGui::ColorEdit3("Light.diffuse", &light[selLight].diffuse.r, flags);
		ImGui::ColorEdit3("Light.specular", &light[selLight].specular.r, flags);
		ImGui::SliderFloat3("Light.position", &light[selLight].position.x, min * mult, max * mult);
		ImGui::SliderFloat3("Light.direction", &light[selLight].direction.x, -1, 1);
		ImGui::SliderFloat("Light.constant", &light[selLight].constant, 0, 1, "%.6f");
		ImGui::SliderFloat("Light.linear", &light[selLight].linear, 0, 1, "%.6f");
		ImGui::SliderFloat("Light.quadratic", &light[selLight].quadratic, 0, 1, "%.6f");
		ImGui::SliderFloat("Light.cutoff", &light[selLight].cutoff, 0, 180);
		ImGui::SliderFloat("Light.outerCutoff", &light[selLight].outerCutoff, 0, 180);
		ImGui::InputInt("Light.type", &light[selLight].type);
		ImGui::SliderFloat("Range multiplier", &mult, 0, 32);
		ImGui::Checkbox("Flashlight", &flashlight);
		if(ImGui::Button("Point light from camera")) {
			light[selLight].position = camera.position;
			light[selLight].direction = camera.front;
		}

		

		ImGui::Text("Camera parameters:");
		ImGui::SliderFloat("Camera.speed", &camera.speed, 0.1, 200);
		if (ImGui::Checkbox("WireFrame mode", &wireframeMode)) {
			glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
		}

		ImGui::Text("Different stuff:");
		ImGui::ColorEdit3("Clear color", &clearColor.x);
		ImGui::Checkbox("Restrict world area?", &restrictArea);

		ImGui::Text("Model parameters");
		if (ImGui::InputText("Path:", &path[0], 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
			sceneObject.clear();
			sceneObject.load(path);
		}

		ImGui::SliderFloat3("Model position", &modelPosition.x, -1 * mult, 1 * mult);
		ImGui::SliderFloat3("Model scale", &modelScale.x, 0, 1 * mult);
		ImGui::SliderFloat3("Model rotation", &modelRotation.x, -180, 180);
		ImGui::SliderFloat("Multiplier", &mult, 0, 10);
		if(ImGui::Button("Set rotation by default")) {
			modelRotation = glm::vec3(0);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		
		
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(game.getWidow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/* ------ ============ ------ */



		game.swapBuffers();

	}
}




// ------------ Different update stuff ------------
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	static float prevX = xPos, prevY = yPos;
	float dx, dy;

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		// Calculating the difference in X & Y plane, to update yaw & pitch
		dx = xPos - prevX;
		dy = yPos - prevY;

		camera.processMouseMovement(dx, dy);

		prevX = xPos;
		prevY = yPos;
	} else {
		prevX = xPos;
		prevY = yPos;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) 
		camera.processMouseScroll(yoffset);
}

void keyboardUpdate(GLFWwindow* window) {
	static float previous = glfwGetTime();
	float now = glfwGetTime();

	float deltaTime = now - previous;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processMovement(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processMovement(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processMovement(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processMovement(DOWN, deltaTime);

	previous = now;
}