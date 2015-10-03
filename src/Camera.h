#pragma once

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::mat4 view;
	glm::mat4 proj;
	Camera();
	glm::mat4 ProjViewMatrix();
};