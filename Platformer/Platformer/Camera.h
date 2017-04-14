#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void initCamera(int width, int height);

	void updateCamera();

	glm::vec2 getPosition();
	void setPosition(glm::vec2& position);

	glm::mat4 getCamerMatrix();

	float getScale();
	void setScale(float scale);

private:
	int mScreenWidth;
	int mScreenHeight;

	bool mUpdateMatrix;

	glm::vec2 mPosition;
	glm::mat4 mOrthographicMatrix;
	glm::mat4 mCameraMatrix;

	float mScale;
};

