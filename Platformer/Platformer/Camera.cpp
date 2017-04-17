#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

//Setup the camera
void Camera::initCamera(int width, int height)
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

		mUpdateMatrix = false;
	}
}

//Get the camera's position (2d vector)
glm::vec2 Camera::getPosition()
{
	return mPosition;
}

//Set the camera's position (2d vector)
void Camera::setPosition(glm::vec2& position)
{
	mPosition = position;

	//Reset the update bool
	mUpdateMatrix = true;
}

//Get the camera matrix
glm::mat4 Camera::getCamerMatrix()
{
	return mCameraMatrix;
}

//Get the camera scale
float Camera::getScale()
{
	return mScale;
}

//Set the camera scale
void Camera::setScale(float scale)
{
	mScale = scale;

	//Reset the update bool
	mUpdateMatrix = true;
}

//Convert screen coords to world coords
glm::vec2 Camera::screenToWorld(glm::vec2 screenCoords)
{
	//invert y
	screenCoords.y = mScreenHeight - screenCoords.y;

	//Centre screen to 0
	screenCoords -= glm::vec2(mScreenWidth / 2, mScreenHeight / 2);

	//Scale coords
	screenCoords /= mScale;

	//Translate using camera position
	screenCoords += mPosition;

	return screenCoords;
}

//True if the entity is off the screen
bool Camera::cullOffScreen(const glm::vec2& position, const glm::vec2& dimensions)
{
	//Scale screen dimensions to mScale
	//glm::vec2 scaledScreenDimensions = glm::vec2(mScreenWidth, mScreenHeight) / (mScale);
	//cull testing
	glm::vec2 scaledScreenDimensions = glm::vec2(mScreenWidth, mScreenHeight) / (mScale * 2.0f);

	//Minimum distance before a collision occurs
	const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
	const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

	//Centre position of the parameters
	glm::vec2 centrePos = position + dimensions / 2.0f;
	//Centre position of the camera
	glm::vec2 centreCameraPos = mPosition;
	//Vector from the input to the camera
	glm::vec2 distVec = centrePos - centreCameraPos;

	//Get the depth of the collision
	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	//If both the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0) 
	{
		//There was a collision
		return true;
	}
	return false;
}

