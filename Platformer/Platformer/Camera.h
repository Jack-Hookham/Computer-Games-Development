#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//2D camera class
//Engine currently uses two cameras - one to follow the player and one to draw the HUD

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

	glm::vec2 screenToWorld(glm::vec2 screenCoords);

	bool isOnCamera(const glm::vec2& position, const glm::vec2& dimensions);

private:
	int mScreenWidth = 1280;
	int mScreenHeight = 720;

	bool mUpdateMatrix = true;

	glm::vec2 mPosition = glm::vec2(0.0f);
	glm::mat4 mOrthographicMatrix = glm::mat4(1.0f);
	glm::mat4 mCameraMatrix = glm::mat4(1.0f);

	float mScale = 1.0f;
};

