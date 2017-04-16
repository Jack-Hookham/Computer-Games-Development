#include "Camera.h"

Camera::Camera() : mScreenWidth(1280), mScreenHeight(720), mUpdateMatrix(true), mPosition(0.0f, 0.0f), mOrthographicMatrix(1.0f), mCameraMatrix(1.0f), mScale(1.0f)
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

