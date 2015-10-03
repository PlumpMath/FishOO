//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//standard libs
#include <stdio.h>
#include <string>
#include <chrono>
#include <iostream>

//my stuff
#include "globals.h"
#include "Camera.h"
#include "Fish.h"

//variables
SDL_Window* window;
SDL_GLContext context;
std::chrono::time_point<std::chrono::high_resolution_clock> t_start;
std::chrono::time_point<std::chrono::high_resolution_clock> t_then;
GLint uniModel;
GLint uniAnim;

//global variables
Camera camera = Camera();
Fish fish(glm::vec3(0, 1, 0), 45.0, 2.0);
Fish fish2(glm::vec3(-0.5, -0.5, 1), -90.0, 1.0);
Fish fish3(glm::vec3(-3, -6, -2), 180.0, 3.0);

void init() {
	//start SDL with video module	
	SDL_Init(SDL_INIT_VIDEO);

	//set OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//create window
	window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

	//create OpenGL context
	context = SDL_GL_CreateContext(window);

	//use glew to find experimental/modern OpenGl function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
}

void exit() {
	//close window & delete context
	SDL_GL_DeleteContext(context);

	SDL_Quit();
}

void update() {
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_then).count();

	//rotate camera
	camera.view = glm::rotate(
			camera.view,
			-1.0f * 0.1f * dt * glm::radians(180.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	fish.update(time);
	fish.draw();

	fish2.update(time);
	fish2.draw();

	fish3.update(time);
	fish3.draw();

	t_then = t_now;
}

//main game loop boilerplate
void loop() {
	//start time
	t_start = std::chrono::high_resolution_clock::now();
	t_then = t_start;

	//event loop
	SDL_Event windowEvent;
	while (true) {

		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) break;
		}

		// Clear the screen to black
        glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //depth buffer needs to be cleared for depth testing

		//update & draw everything
		update();

		//update the window
		SDL_GL_SwapWindow(window);
	}
}

int main(int argc, char *argv[])
{

	init();

	Fish::InitModel();

	//main loop
	loop();

	Fish::DestroyModel();

    exit();
	
	return 0;

}