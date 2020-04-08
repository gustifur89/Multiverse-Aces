#include "CameraFollower.h"

CameraFollower::CameraFollower()
{
	target = std::shared_ptr<GameObject>(NULL);
	followingDistance = 0.0;
}

CameraFollower::~CameraFollower()
{
}

void CameraFollower::setTarget(std::shared_ptr<GameObject> target)
{
	this->target = target;
}

void CameraFollower::setFollowingDistance(double distance)
{
	followingDistance = distance;
}

void CameraFollower::updateCamera(Camera& camera)
{
	/*
	glm::vec3 lookDirection = glm::vec3(0, 0, 1);

	glm::vec4 offset(0, 6, -10, 1);
	

	//get location...  project it into the target's frame

	glm::mat4 coordTransform = target->transform.getTransform();

	glm::vec4 newPos = coordTransform * offset;

	if (!target)
	{

	}
	
	glm::vec3 up = target->transform.getTransformedY();
	
	//camera.position = glm::vec3(newPos);// target->transform.position;// PhysicsBody::transform.position;
	//camera.setRotation(target->transform);
	*/

	camera.position = this->position;
	camera.setRotation(*this);

	//glm::vec4 lookDirection(0, 0, 1, 0);	//Looking direction in viewSpace
	//glm::vec4 upDirection(0, 1, 0, 0);
	////rotate by the direction
	//glm::mat4 rotationMatrix = this->getRotation();// glm::toMat4(glm::quat((float)TO_RAD * rotation));
	////roate the look direction by the rotation
	//lookDirection = rotationMatrix * lookDirection;
	//upDirection = rotationMatrix * upDirection;
	//glm::vec3 lookAt = position + glm::vec3(lookDirection);

	//return glm::lookAt(
	//	position,	// the position of your camera, in world space
	//	lookAt,		// where you want to look at, in world space
	//	up// glm::vec3(0, 1, 0)    // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	//);

}

void CameraFollower::move(double deltaTime)
{
	if (target)
	{
		glm::vec4 offset(0, 6, -10, 1);

		glm::mat4 coordTransform = target->transform.getTransform();

		glm::vec3 goalPos = glm::vec3(coordTransform * offset);

		glm::vec3 up = target->transform.getTransformedY();

		double p = 4.0;

		glm::vec3 err = goalPos - position;

		position += (float) (deltaTime * p) * err;
		

		//target->transform.rotation;
		this->rotation = glm::slerp(this->rotation, target->transform.rotation, (float)deltaTime * 4.0f);// (float)deltaTime * 0.0001f);// (float)deltaTime * 0.90f);


		//setRotation(target->transform);
	}
}
