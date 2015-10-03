#include "Camera.h"
#include <iostream>

Camera::Camera() {
	//init public variables to defaults
	std::cout << "init new camera" << std::endl;
	
	//set up camera view
	view = glm::lookAt(
	    glm::vec3(2.0f, 1.2f, 2.0f),
	    glm::vec3(0.0f, 0.0f, 0.0f),
	    glm::vec3(0.0f, 1.0f, 0.0f)
	);	

	//set up field of view / projection matrix
	proj = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 1.0f, 10.0f);
}

glm::mat4 Camera::ProjViewMatrix() {
	return proj * view;
}