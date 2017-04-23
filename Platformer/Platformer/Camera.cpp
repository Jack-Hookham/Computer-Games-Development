#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

//Setup the camera
void Camera::initCamera(const int width, const int height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	//Initialise the orthographic matrix
	mOrthographicMatrix = glm::ortho(0.0f, (float)mScreenWidth, 0.0f, (float)mScreenHeight);
}

//Update the camera
void Camera::updateCamera()
{
	if (mUpdateMatrix)
	{
		//Translate the camera
		//glm::vec3 translate(-mPosition.x, -mPosition.y, 0.0f);
		glm::vec3 translate(-mPosition.x + mScreenWidth / 2, -mPosition.y + mScreenHeight / 2, 0.0f);
		mCameraMatrix = glm::translate(mOrthographicMatrix, translate);

		//Scale camera
		glm::vec3 scale(mScale, mScale, 0.0f);
		mCameraMatrix = glm::scale(glm::mat4(1.0f), scale) * mCameraMatrix;

		//No longer need to update the camera matrix
		mUpdateMatrix = false;
	}
}

//Set the camera's position
void Camera::setPosition(const glm::vec2& position)
{
	mPosition = position;

	//Reset the update bool
	mUpdateMatrix = true;
}

//Set the camera scale
void Camera::setScale(const float scale)
{
	mScale = scale;

	//Reset the update bool
	mUpdateMatrix = true;
}

//Convert screen coords to world coords
glm::vec2 Camera::screenToWorld(const glm::vec2 screenCoords)
{
	glm::vec2 worldCoords = screenCoords;

	//invert y
	worldCoords.y = mScreenHeight - screenCoords.y;

	//Centre screen to 0, 0
	worldCoords -= glm::vec2(mScreenWidth / 2, mScreenHeight / 2);

	//Scale coords
	worldCoords /= mScale;

	//Translate using camera position
	worldCoords += mPosition;

	return worldCoords;
}

//True if the entity is off the camera
//Used for culling off screen entities
//Calculates whether the camera view collides with the entity
bool Camera::isOnCamera(const glm::vec2& position, const glm::vec2& dimensions)
{
	//Scale screen dimensions to mScale
	glm::vec2 scaledScreenDimensions = glm::vec2(mScreenWidth, mScreenHeight) / mScale;

	//cull testing (multiply scale)
	//glm::vec2 scaledScreenDimensions = glm::vec2(mScreenWidth, mScreenHeight) / (mScale * 2.0f);

	//Minimum distance before a collision occurs
	float minDistanceX = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
	float minDistanceY = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

	//Centre position of the entity
	glm::vec2 centreEntityPos = position + dimensions / 2.0f;

	//Centre position of the camera
	glm::vec2 centreCameraPos = mPosition;

	//Distance from the input to the camera
	glm::vec2 distVec = centreEntityPos - centreCameraPos;

	//Get the depth of the collision
	float xDist = minDistanceX - abs(distVec.x);
	float yDist = minDistanceY - abs(distVec.y);

	//If both the distances are > 0, then there was a collision and the entity is on screen, so it shouldn't be culled
	if (xDist > 0 && yDist > 0)
	{
		return true;
	}
	//Otherwise the entity should be culled
	return false;
}

