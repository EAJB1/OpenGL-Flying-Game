// The LearnOpenGL (https://learnopengl.com/) tutorial by Joey de Vries was used in this project, changes to code have been made.
// All code used from LearnOpenGL falls under the CC BY-NC 4.0 license (https://creativecommons.org/licenses/by-nc/4.0/).

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Helper.h"
#include "Skybox.h"
#include "Camera.h"
#include "GameObject.h"
#include "Ring.h"
#include "Planet.h"
#include "Rock.h"

GLFWwindow* Start();
void GetMonitorResolution();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Render settings
#define	WINDOW_WIDTH 1024
#define	WINDOW_HEIGHT 576
#define FULL_SCREEN false
#define MAXIMISED true
#define WIREFRAME false
#define NEAR 0.1f // Distance to near clipping plane
#define FAR 4000.0f // Distance to far clipping plane

// Resolution of primary monitor
GLint monitorWidth;
GLint monitorHeight;

// Time
float deltaTime = 0.0f; // Time between each frame
float lastFrame = 0.0f; // Time stamp of the last frame

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
glm::vec3 lastPosition = vec3Zero;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// File paths
const char* earthPath = "../Assets/earth/earth.obj";
const char* ringPath = "../Assets/ring/stargate.obj";
const char* asteroidPath = "../Assets/rocks/asteroid.obj";
const char* asteroid2Path = "../Assets/rocks/asteroid2.obj";
const char* asteroid3Path = "../Assets/rocks/asteroid3.obj";
const char* asteroid4Path = "../Assets/rocks/asteroid4.obj";
const char* asteroid5Path = "../Assets/rocks/asteroid5.obj";
const char* asteroid6Path = "../Assets/rocks/asteroid6.obj";

std::vector<std::string> faces
{
	"../Assets/skybox/px.png", // Right
	"../Assets/skybox/nx.png", // Left
	"../Assets/skybox/py.png", // Top
	"../Assets/skybox/ny.png", // Bottom
	"../Assets/skybox/pz.png", // Back
	"../Assets/skybox/nz.png"  // Front
};

int main()
{
	GLFWwindow* window = Start();

	// Stop program if window is not created
	if (window == NULL)
	{
		return -1;
	}

	// Shaders
	Shader shader("MaterialsVert.glsl", "MaterialsFrag.glsl");
	Shader skyboxShader("SkyboxVert.glsl", "SkyboxFrag.glsl");
	
	// Skybox
	Skybox* skybox = new Skybox(faces, &skyboxShader);

	// Models
	Model earthModel(earthPath);

	Model asteroidModel(asteroidPath);
	Model asteroid2Model(asteroid2Path);
	Model asteroid3Model(asteroid3Path);
	Model asteroid4Model(asteroid4Path);
	Model asteroid5Model(asteroid5Path);
	Model asteroid6Model(asteroid6Path);

	std::vector<Model*> asteroidModels;
	asteroidModels.push_back(&asteroidModel);
	asteroidModels.push_back(&asteroid2Model);
	asteroidModels.push_back(&asteroid3Model);
	asteroidModels.push_back(&asteroid4Model);
	asteroidModels.push_back(&asteroid5Model);
	asteroidModels.push_back(&asteroid6Model);

	Model ringModel(ringPath);
	
	// GameObjects
	// Planets
	Planet* earth = new Planet(&earthModel, &shader);
	earth->position = glm::vec3(0.0f, 0.0f, -2000.0f);
	earth->scale = glm::vec3(1000.0f);
	earth->GenerateAsteroidBelt(asteroidModels, &shader);

	// Ring
	Ring* startRing = new Ring(&ringModel, &shader);
	startRing->position = glm::vec3(0.0f, 0.0f, -10.0f);
	startRing->scale = 5.0f * vec3One;

	Ring::rings.push_back(startRing);
	Ring::currentRing = startRing;

	Ring::SpawnRing(&ringModel, &shader);

	Ring::SpawnRing(&ringModel, &shader);

	// Define shaders
	shader.use();
	
	shader.setVec3("light.direction", glm::vec3(1.0f, -1.0f, -1.0f));
	shader.setVec3("light.ambient", vec3One);
	shader.setVec3("light.diffuse", vec3One);
	shader.setVec3("light.specular", vec3One);

	shader.setVec3("material.ambient", vec3One * 0.2f);
	shader.setVec3("material.diffuse", vec3One);
	shader.setVec3("material.specular", vec3One * 0.5f);
	shader.setFloat("material.shininess", 32.0f);

	// Program render loop
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Time
		
		float frameTime = static_cast<float>(glfwGetTime());
		deltaTime = frameTime - lastFrame;
		lastFrame = frameTime;

		#pragma endregion

		// Handle input
		processInput(window);
		
		#pragma region Update

		camera.Update(deltaTime);
		
		for (GameObject* g : GameObject::gameObjects)
		{
			g->Update(deltaTime);
		}
		
		#pragma endregion

		#pragma region Collision

		// If collided with oldest ring, spawn next ring.
		if (Ring::rings[0]->Collision(lastPosition, camera.position))
		{
			camera.ringsPassed++;

			Ring::SpawnRing(&ringModel, &shader);

			camera.PerformBoost();
		}

		lastPosition = camera.position;
		
		#pragma endregion
		
		#pragma region Rendering
		
		// Fill screen with a solid colour
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		// Camera projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, NEAR, FAR);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		shader.setVec3("viewPos", camera.position);
		
		GameObject::DrawAll();

		// Draw Skybox last
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skybox->Update();

		#pragma endregion

		glfwSwapBuffers(window);

		// Poll IO events are for key pressed/released or mouse movement.
		glfwPollEvents();
	}

	GameObject::DeleteAll();
	skybox->Delete();

	// Delete all of GLFW's resources.
	glfwTerminate();

	return 0;
}

GLFWwindow* Start()
{
	// Initialise GLFW
	glfwInit();

	// Configure GLFW
	// The first parameter is the enum that will be changed, the second is the value it will be set to.

	// Specifiy the client API version, major and minor version is set to GLFW version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// GLFW will use core-profile that accesses OpenGl features without backwards-compatible features that are not needed.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Update monitor resolution depending on being in full screen.
	monitorWidth = WINDOW_WIDTH;
	monitorHeight = WINDOW_HEIGHT;

	if (FULL_SCREEN)
	{
		GetMonitorResolution();
	}

	// The window object holds all window data.
	GLFWwindow* window = glfwCreateWindow(monitorWidth, monitorHeight, "2203760", FULL_SCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Update mouse position and scroll values.
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW will capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load the address of the OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	// Tell OpenGL the size of the rendering window
	// The first two parameters set the location of the lower left corner of the window.
	// (The glad viewport dimensions can be smaller than GLFW's and the window will be rendered in a smaller window)
	glViewport(0, 0, monitorWidth, monitorHeight);

	if (MAXIMISED)
	{
		glfwMaximizeWindow(window);
	}

	if (WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// The callback function is called when the window is first dislayed.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return window;
}

/// <summary>
/// Sets width and height to primary monitor resolution.
/// </summary>
void GetMonitorResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	monitorWidth = mode->width;
	monitorHeight = mode->height;
}

/// <summary>
/// Update camera position using mouse movement input.
/// </summary>
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	// Set x and y to current mouse position if first mouse callback,
	// stops the camera jumping when entering the window at different positions.
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

/// <summary>
/// Update camera zoom using scroll wheel offset.
/// </summary>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

/// <summary>
/// Return if the key is currently being pressed.
/// </summary>
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT);
	if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN);
	
	/*if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.Boost = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.Boost = false;*/
}

/// <summary>
/// Called each time the window is resized.
/// </summary>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
