#pragma once

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//SDL open gl
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//other
#include <string>

class Fish {

	public:
		// load / unload model from memory
		static void InitModel();
		static void DestroyModel();
		static std::string readFile(const char* filePath);

		//variables
		glm::mat4 model; //transform for the fish
		float animSpeed;

		//methods
		Fish(glm::vec3 pos, float rot, float _animSpeed);
		void update(float t);
		void draw();

	private:
		//shaders
		/*
		static const GLchar* vertexSource;
		static const GLchar* fragmentSource;
		*/

		//gl stuff
		static GLuint vao;
		static GLuint vbo;
		static GLuint ebo;

		//shaders
		static GLuint vertexShader;
		static GLuint fragmentShader;
		static GLuint shaderProgram;

		//uniforms
		static GLint uniProj;
		static GLint uniView;
		static GLint uniModel;
		static GLint uniAnim;
};