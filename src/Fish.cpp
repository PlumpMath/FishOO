#include "Fish.h"

#include "Camera.h"
#include "globals.h"

#include <iostream>

// Shader sources
const GLchar* Fish::vertexSource =
	"#version 150\n"
	"in vec3 position;"
	"in vec3 normal;"
	"uniform mat4 proj;"
	"uniform mat4 view;"
	"uniform mat4 model;"
	"uniform float animationOffset;"
	"out vec3 color;"
	"void main()" 
	"{"
	"	vec3 lightV = normalize(vec3(1,-1,0));"
	"	vec3 mvNorm = vec3(model * vec4(normal, 0));"
	"	float diffuse = max(dot(mvNorm, lightV), 0.3);"
	"	vec3 startColor = vec3(1.0, 0.5, 0.0);"
	"	color = startColor * diffuse;"
	"	vec3 newPos = position + vec3(0, 0, (sin((animationOffset + position.x) * 4) / 16));"
	"	gl_Position = proj * view * model * vec4(newPos, 1.0);"
	"}";

const GLchar* Fish::fragmentSource =
    "#version 150\n"
    "in vec3 color;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(color, 1.0);"
    "}";

//this feels like a weird hack, but all these variables need to be instantiated
GLuint Fish::vao = 0;
GLuint Fish::vbo = 0;
GLuint Fish::ebo = 0;
GLuint Fish::vertexShader = 0;
GLuint Fish::fragmentShader = 0;
GLuint Fish::shaderProgram = 0;
GLint Fish::uniProj = 0;
GLint Fish::uniView = 0;
GLint Fish::uniModel = 0;
GLint Fish::uniAnim = 0;

void Fish::InitModel() {

	std::cout << "test static" << std::endl;

	//another weird hack - couldn't figure out how to define the static arrays
	//model data
	GLfloat vertices[] = {

		//0
		//front vertex
		-0.7, 0, 0, 0, 0, 0,

		//1 - 4
		//left side
		-0.4, -0.25, 0.1, 0, 0, 0,
		-0.4, 0.25, 0.1, 0, 0, 0,
		-0.1, 0.3, 0.2, 0, 0, 0,
		-0.1, -0.3, 0.2, 0, 0, 0,

		//5 - 8
		//right side
		-0.4, -0.25, -0.1, 0, 0, 0,
		-0.4, 0.25, -0.1, 0, 0, 0,
		-0.1, 0.3, -0.2, 0, 0, 0,
		-0.1, -0.3, -0.2, 0, 0, 0,

		//9 - 12
		//tail
		0.25, -0.25, 0, 0, 0, 0,
		0.25, 0.25, 0, 0, 0, 0,
		0.7, 0.5, 0, 0, 0, 0,
		0.7, -0.5, 0, 0, 0, 0

	};


	GLuint elements[] = {

		//left side
		0, 1, 2,
		1, 3, 2,
		1, 4, 3,
		4, 9, 3,
		9, 10, 3,

		//tail
		9, 11, 10,
		11, 12, 9,

		//right side
		0, 6, 5,
		5, 6, 7,
		5, 7, 8,
		7, 9, 8,
		7, 10, 9,

		//top
		2, 6, 0,
		2, 7, 6,
		2, 3, 7,
		3, 10, 7,

		//bottom
		0, 5, 1,
		1, 5, 8,
		1, 4, 8,
		9, 4, 8

	};

	//init and bind Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//calculate face normals
	for (int t = 0; t < sizeof(elements) / sizeof(GLuint); t += 3) {

		//std::cout << t << std::endl;
		
		int t0 = elements[t];
		int t1 = elements[t+1];
		int t2 = elements[t+2];

		glm::vec3 vert0 = glm::vec3(vertices[ (t0 * 6) + 0 ], vertices[ (t0 * 6) + 1 ], vertices[ (t0 * 6) + 2 ]);
		glm::vec3 vert1 = glm::vec3(vertices[ (t1 * 6) + 0 ], vertices[ (t1 * 6) + 1 ], vertices[ (t1 * 6) + 2 ]);
		glm::vec3 vert2 = glm::vec3(vertices[ (t2 * 6) + 0 ], vertices[ (t2 * 6) + 1 ], vertices[ (t2 * 6) + 2 ]);

		glm::vec3 side0 = vert0 - vert2;
		glm::vec3 side1 = vert1 - vert2;

		glm::vec3 faceNorm = glm::cross(side0, side1);

		//std::cout << faceNorm.x << " "  << faceNorm.y  << " "  << faceNorm.z << std::endl;

		vertices[ (t0 * 6) + 0 + 3 ] += faceNorm.x;
		vertices[ (t0 * 6) + 1 + 3 ] += faceNorm.y;
		vertices[ (t0 * 6) + 2 + 3 ] += faceNorm.z;

		vertices[ (t1 * 6) + 0 + 3 ] += faceNorm.x;
		vertices[ (t1 * 6) + 1 + 3 ] += faceNorm.y;
		vertices[ (t1 * 6) + 2 + 3 ] += faceNorm.z;

		vertices[ (t2 * 6) + 0 + 3 ] += faceNorm.x;
		vertices[ (t2 * 6) + 1 + 3 ] += faceNorm.y;
		vertices[ (t2 * 6) + 2 + 3 ] += faceNorm.z;
	}

	//re-normalize vertex normals
	for (int v = 0; v < sizeof(vertices) / sizeof(GLuint); v += 6) {
		glm::vec3 norm = glm::normalize( glm::vec3(vertices[v + 3], vertices[v + 4], vertices[v + 5]) );

		vertices[v + 3] = norm.x;
		vertices[v + 4] = norm.y;
		vertices[v + 5] = norm.z;
	}

	//create vertex buffer
	glGenBuffers(1, &vbo);
	//make buffer the active object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//send vertex data to vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create element buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**) &vertexSource, NULL);
	glCompileShader(vertexShader);

	//fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**) &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//make shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	//link the program to make it "compile"
	glLinkProgram(shaderProgram);
	//actually use the shader program
	glUseProgram(shaderProgram);

	//tell position attribute how to get data from vertices
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
	
	//tell normal attribute how to get data from vertices
	GLint normAttrib = glGetAttribLocation(shaderProgram, "normal");
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));

	//get uniforms for the various transformation matrices
	uniProj = glGetUniformLocation(shaderProgram, "proj");
	uniView = glGetUniformLocation(shaderProgram, "view");
	uniModel = glGetUniformLocation(shaderProgram, "model");

	//get uniform for wiggling animation
	uniAnim = glGetUniformLocation(shaderProgram, "animationOffset");
}

void Fish::DestroyModel() {
	//delete gl stuff
	glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &vao);	
}

Fish::Fish(glm::vec3 pos, float rot, float _animSpeed) {
	//nothin' to do here
	std::cout << "test fish" << std::endl;

	//apply position
	model = glm::translate(model, pos);

	//apply rotation
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

	//set animation speed
	animSpeed = _animSpeed;
}

void Fish::update(float t) {
	//animate swimming fish
	glUniform1f(uniAnim, animSpeed * t);
}

void Fish::draw() {
	//use the correct VAO
	glBindVertexArray(vao);

	//update camera
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

	//update model transform
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	//draw the fish
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
}