// TileScroller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef STB_IMAGE_IMPLEMENTATION // Causes errors if in Config.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <iostream>
#include "opengl_libs.h"
#include "Shader.h"
#include "Sprite.h"

// Callbacks
unsigned int screenx = 800;
unsigned int screeny = 600;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void error_callback(int error, const char* description);

int main()
{
	// Setup
		// GLFW Initilisation
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// Window Creation
	GLFWwindow* window = glfwCreateWindow(screenx, screeny, "openGL Tutorial", NULL, NULL); // Make window
	if (window == NULL) // Window failed
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // End program
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Resize the window with framebuffer_size_callback()
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetErrorCallback(error_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initilize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate(); // End program
		return -1;
	}

	// Declare window size and GL settings
	glViewport(0, 0, screenx, screeny);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // Transparencey isn't perfect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	// Shaders
	Shader spriteshader("Spriteshader.vert", "Spriteshader.frag");

	Spritesheet sheet("wall.jpg", glm::ivec2(512, 512));
	Render_Batch batch(&sheet);
	Sprite test_sprite(&batch, 1, glm::vec2(0.0f), glm::vec2(1.0f));


	bool should_close = false;
	while (!should_close) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		spriteshader.use();
		// render here
		batch.render();
		spriteshader.dontuse();

		glfwSwapBuffers(window);
		glfwPollEvents();
		should_close = glfwWindowShouldClose(window);
	}

	// Cleanup
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { // Resizes window when needed
	screenx = width;
	screeny = height;
	glViewport(0, 0, width, height);
}

double lastmousex = screenx / 2.0;
double lastmousey = screeny / 2.0;
bool firstmousecallback = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}