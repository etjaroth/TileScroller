// TileScroller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef STB_IMAGE_IMPLEMENTATION // Causes errors if in Config.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <iostream>
#include "opengl_libs.h"
#include "Shader.h"
#include "Entity.h"
#include "Camera.h"
#include "Tileset.h"
#include "Scroller.h"

// Callbacks
unsigned int screenx = 800;
unsigned int screeny = 600;

glm::vec2 mouse_pos2 = glm::vec2(0.0f, 0.0f);
glm::vec4 mouse_pos4 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
int palate = 0;
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetErrorCallback(error_callback);

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

	// Rendering
	const float camera_size = 1.05f;
	Camera camera = Camera();
	Shader spriteshader("Spriteshader.vert", "Spriteshader.frag");
	camera.set_size(glm::vec2(camera_size));
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 view = camera.get_view();
	glm::mat4 projection = glm::scale(glm::mat4(1.0f), glm::vec3((float)screeny / (float)screenx, 1.0f, 1.0f));

	glm::mat4 inv_model = glm::inverse(model);
	glm::mat4 inv_view = glm::inverse(view);
	glm::mat4 inv_projection = glm::inverse(projection);

	spriteshader.set_mat4("model", model);
	spriteshader.set_mat4("view", view);
	spriteshader.set_mat4("projection", projection);
	spriteshader.dontuse();

	// Make a game world
	Scroller world(glm::ivec2(-50, 50), glm::ivec2(100), "tile_spritesheet.png", glm::ivec2(16));
	//Scroller world(glm::vec2(-100, 100), glm::ivec2(200), "tile_spritesheet.png", glm::ivec2(16));

	Spritesheet pc_sheet("Player.png", glm::ivec2(16));
	Render_Batch pc_batch(&pc_sheet);
	std::shared_ptr<Entity> player = std::make_shared<Entity>(&pc_batch, 0, glm::vec2(-0.25f, 0.25f), glm::vec2(0.5f));
	player->set_depth(0.5f);
	player->name = "player";
	player->activate();
	player->set_physics(0.1f, 1.0f, true);

	world.insert(player);

	// to keep the view centred on the player
	glm::vec2 scroller_offset = player->get_pos() - world.get_pos();
	glm::vec2 camera_offset = player->get_pos() - camera.get_pos();

	// Main Loop
	bool should_close = false;
	bool paused = false;
	bool pauseable = true;
	while (!should_close) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Input
		glm::vec2 move = glm::vec2(0.0f);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // exit on escape
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			move.y += 1;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			move.y -= 1;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			move.x -= 1;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			move.x += 1;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			player->change_depth(0.05f);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			player->change_depth(-0.05f);

		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			world.save();
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && pauseable) {
			paused = !paused;
			pauseable = false;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			pauseable = true;;
		}

		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			palate = 0;
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			palate = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			palate = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			palate = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			palate = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			palate = 5;
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			palate = 6;
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			palate = 7;
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			palate = 8;
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			palate = 9;

		glm::vec2 mouse_world_pos = glm::vec2(inv_model * inv_view * inv_projection * (mouse_pos4));



		// Do stuff based on input
		if (!paused) {

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
				world.write_tile(mouse_world_pos, palate);
				//world.write_tile(player.get_pos(), palate);
				/*for (int i = 1; i <= 10; ++i) {
					world.write_tile(player.get_pos() + glm::vec2(i, 0), palate);
					world.write_tile(player.get_pos() + glm::vec2(-i, 0), palate);
					world.write_tile(player.get_pos() + glm::vec2(0, i), palate);
					world.write_tile(player.get_pos() + glm::vec2(0, -i), palate);
				}*/
			}

			move *= 1 - camera_size;
			//move *= 0.1;
			int flip = camera_size >= 1 ? 1 : -1;
			move *= flip;

			if (glm::length(player->get_velocity()) < 0.01f) {
				player->change_velocity(-move);
			}

			world.collide();
			world.iterate();

			player->set_velocity(0.5f * player->get_velocity());
			//player->set_velocity(0.0f* player->get_velocity());
			if (glm::length(player->get_velocity()) < 0.00001f) {
				player->set_velocity(glm::vec2(0.0f));
				//player->set_velocity(glm::vec2(0.0f, -0.05f));
			}

			std::cout << "\n";
			// Move view
			world.set_pos(player->get_pos() - scroller_offset);
			camera.set_pos(camera_offset - player->get_pos());
			//std::cout << player->get_pos().x << ", " << player->get_pos().y << std::endl;
			//std::cout << player->get_velocity().x << ", " << player->get_velocity().y << std::endl;

		}

		// Render
		spriteshader.use();
		view = camera.get_view();
		inv_view = glm::inverse(view);
		spriteshader.set_mat4("view", view);

		world.render();
		pc_batch.render();
		spriteshader.dontuse();

		// hhh
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		if (w != screenx || h != screeny) {
			screenx = w;
			screeny = h;

			glm::mat4 proj = glm::scale(glm::mat4(1.0f), screenx > screeny ? glm::vec3((float)screeny / (float)screenx, 1.0f, 1.0f) : glm::vec3(1.0f, (float)screenx / (float)screeny, 1.0f));
			spriteshader.use();
			spriteshader.set_mat4("projection", proj);
			spriteshader.dontuse();

			glViewport(0, 0, screenx, screeny);
		}

		// Swap Buffers, Call Events
		glfwSwapBuffers(window);
		glfwPollEvents();
		should_close = glfwWindowShouldClose(window);
	}

	// Cleanup
	glfwTerminate();
	return 0;
}



double lastmousex = screenx / 2.0;
double lastmousey = screeny / 2.0;
bool firstmousecallback = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	xpos -= (double)screenx / 2.0;
	ypos -= (double)screeny / 2.0;

	xpos = xpos / ((double)screenx / 2.0);
	ypos = ypos / ((double)screeny / 2.0);

	ypos = -ypos;

	mouse_pos4 = glm::vec4(xpos, ypos, 0.0f, 1.0f);
	mouse_pos2 = glm::vec2(xpos, ypos);
}


void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}