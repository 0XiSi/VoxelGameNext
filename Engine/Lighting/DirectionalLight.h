#pragma once
#include "Light.h"
class DirectionalLight :
	public Light {

public:

	DirectionalLight();
	DirectionalLight(
		GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir
		);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);
	void setDirection(glm::vec3 position) { direction = -position; }
	glm::vec3 getDirection() { return -direction; }

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction;

};

