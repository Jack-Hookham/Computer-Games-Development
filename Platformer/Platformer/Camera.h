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

	//Initialise the camera
	void initCamera(const int width, const int height);

	//Update the camera matrix if needed
	void updateCamera();

	const glm::vec2 getPosition() const { return mPosition; }
	void setPosition(const glm::vec2& position);

	const glm::mat4 getCamerMatrix() const { return mCameraMatrix; }

	const float getScale() const { return mScale; }
	void setScale(const float scale);

	//Convert screen coords to world coords
	glm::vec2 screenToWorld(const glm::vec2 screenCoords);

	//Check whether an entity is inside the camera boundaries
	bool isOnCamera(const glm::vec2& position, const glm::vec2& dimensions);

private:
	int mScreenWidth = 1280;
	int mScreenHeight = 720;

	//Whether the camera needs updating
	bool mUpdateMatrix = true;

	glm::vec2 mPosition = glm::vec2(0.0f);
	glm::mat4 mOrthographicMatrix = glm::mat4(1.0f);
	glm::mat4 mCameraMatrix = glm::mat4(1.0f);
	float mScale = 1.0f;
};

