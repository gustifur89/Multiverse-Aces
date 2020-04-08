#include "PlayerExtension.h"

// =============================== NewEntity ===============================

// ================================ PlayerExtension ==========================

PlayerExtension::PlayerExtension(UIManager* UI) : NewEntity(UI)
{

}

void PlayerExtension::move()
{
	/*
	if (UI->isKeyPressed(GLFW_KEY_V))
	{
		if (noClipToggle)
		{
			noClip = !noClip;
		}
		noClipToggle = false;
	}
	else
	{
		noClipToggle = true;
	}
	*/

	bool w = UI->isKeyPressed(GLFW_KEY_W);
	bool s = UI->isKeyPressed(GLFW_KEY_S);
	bool a = UI->isKeyPressed(GLFW_KEY_A);
	bool d = UI->isKeyPressed(GLFW_KEY_D);

	bool e = UI->isKeyPressed(GLFW_KEY_E);
	bool q = UI->isKeyPressed(GLFW_KEY_Q);

	bool space = UI->isKeyPressed(GLFW_KEY_SPACE);
	bool ctrl = UI->isKeyPressed(GLFW_KEY_LEFT_CONTROL);

	bool up = UI->isKeyPressed(GLFW_KEY_UP);
	bool down = UI->isKeyPressed(GLFW_KEY_DOWN);
	bool right = UI->isKeyPressed(GLFW_KEY_RIGHT);
	bool left = UI->isKeyPressed(GLFW_KEY_LEFT);

	double forwardSpeed = 0.0;
	double strafeSpeed = 0.0;
	double verticalSpeed = 0.0;

	//MOVEMENT
	if (w == s)
		forwardSpeed = 0.0;
	else if (w)
		forwardSpeed = 10.0;
	else
		forwardSpeed = -10.0;

	if (a == d)
		strafeSpeed = 0.0;
	else if (a)
		strafeSpeed = 10.0;
	else
		strafeSpeed = -10.0;

	if (space == ctrl)
		verticalSpeed = 0.0;
	else if (ctrl)
		verticalSpeed = -10.0;
	else
		verticalSpeed = 10.0;

	if (UI->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		forwardSpeed *= 2.0;
		strafeSpeed *= 2.0;
		verticalSpeed *= 2.0;
	}

	//LOOK

	glm::vec3 dRotation(0);

	if (UI->getMouseLockState()) {
		double deltaPitch = UI->deltaMouseY * Settings::mouseSensitivity;
		double deltaPivot = UI->deltaMouseX * Settings::mouseSensitivity;
		dRotation.x = deltaPitch;
		dRotation.z = deltaPivot;
		//lookRotation.x += deltaPitch;
		//lookRotation.z += deltaPivot;
		//if (lookRotation.x > 89.9) lookRotation.x = 89.9;
	//	if (lookRotation.x < -89.9) lookRotation.x = -89.9;
	}

	/*
	double rollSpeed = 0.0;
	if (e == q)
		rollSpeed = 0;
	else if (q)
		rollSpeed = -10.0;
	else
		rollSpeed = 10.0;

	lookRotation.z += NewEntity<T>::UI->deltaTime * rollSpeed;
	*/

	//GameObject::transform.rotation = lookRotation;
	//this->transform.setRotation(lookRotation);// ((float)NewEntity<T>::UI->deltaTime * glm::vec3(pitchSpeed, yawSpeed, rollSpeed));
	this->transform.rotate(dRotation);

	glm::vec3 directionForward = this->transform.getTransformedZ();
	glm::vec3 directionStrafe = this->transform.getTransformedX();
	glm::vec3 directionVertical = this->transform.getTransformedY();

	this->velocity = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
	//if (NewEntity<T>::UI->isKeyPressed(GLFW_KEY_L))
	this->transform.position += (float) UI->deltaTime * this->velocity;

	/*
	if (noClip)
	{
		vel = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
		this->velocity = vel;
	}
	else
	{


		if (floorColliding)
		{
			onGroundTime = 0.0;
			hasJump = true;
		}
		if (roofColliding)
		{

		}

		if (hasJump && onGroundTime <= Settings::onGroundTimeMax)
		{

			onGroundTime += UI->deltaTime;
		}

		if (jumpToggle.toggle(UI->isKeyPressed(GLFW_KEY_SPACE)))
		{
			this->applyImpulse(glm::vec3(0, 10, 0), glm::vec3(0));
			if (hasJump)
				jumpTime = 0.0;
			hasJump = false;
			if (jumpTime <= jumpTimeMax)
			{
				jumpTime += UI->deltaTime;
			}
		}
		else
		{
			jumpTime = jumpTimeMax + 1.0;
		}


		vel.z = cos(lookRotation.y * TO_RAD) * forwardSpeed - sin(lookRotation.y * TO_RAD) * strafeSpeed;
		vel.x = sin(lookRotation.y * TO_RAD) * forwardSpeed + cos(lookRotation.y * TO_RAD) * strafeSpeed;

		this->velocity.x = vel.x;
		this->velocity.z = vel.z;
	}

	//std::cout << collisionStructure.collide(collider).size() << "\n";

	if (UI->isKeyPressed(GLFW_KEY_3))
	{
		PhysicsBody::transform.position = glm::vec3(0, 10.0, 0);
	}

	//Entity::move(collisionStructure);

	if (held != nullptr)
	{
		held->setTransform(PhysicsBody::transform);
	}
	*/
}

void PlayerExtension::move(std::shared_ptr<Joystick> joystick)
{

	if (noClipToggle.toggle(UI->isKeyPressed(GLFW_KEY_V)))
		noClip = !noClip;
	
	bool w = UI->isKeyPressed(GLFW_KEY_W);
	bool s = UI->isKeyPressed(GLFW_KEY_S);
	bool a = UI->isKeyPressed(GLFW_KEY_A);
	bool d = UI->isKeyPressed(GLFW_KEY_D);

	bool e = UI->isKeyPressed(GLFW_KEY_E);
	bool q = UI->isKeyPressed(GLFW_KEY_Q);

	bool space = UI->isKeyPressed(GLFW_KEY_SPACE);
	bool ctrl = UI->isKeyPressed(GLFW_KEY_LEFT_CONTROL);

	bool up = UI->isKeyPressed(GLFW_KEY_UP);
	bool down = UI->isKeyPressed(GLFW_KEY_DOWN);
	bool right = UI->isKeyPressed(GLFW_KEY_RIGHT);
	bool left = UI->isKeyPressed(GLFW_KEY_LEFT);

	double forwardSpeed = 0.0;
	double strafeSpeed = 0.0;
	double verticalSpeed = 0.0;

	double speed = 40.0;
	double rotationSpeed = 160.0;

	//MOVEMENT
	if (w == s)
		forwardSpeed = 0.0;
	else if (w)
		forwardSpeed = speed;
	else
		forwardSpeed = -speed;

	if (a == d)
		strafeSpeed = 0.0;
	else if (a)
		strafeSpeed = speed;
	else
		strafeSpeed = -speed;

	if (space == ctrl)
		verticalSpeed = 0.0;
	else if (ctrl)
		verticalSpeed = -speed;
	else
		verticalSpeed = speed;

	if (UI->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		forwardSpeed *= 2.0;
		strafeSpeed *= 2.0;
		verticalSpeed *= 2.0;
	}

	//LOOK

	glm::vec3 dRotation(0);
	dRotation.x = -joystick->getAxis(1);
	dRotation.z = joystick->getAxis(0);

	
	dRotation = (float)( UI->deltaTime * rotationSpeed) * dRotation;

	//std::cout << glm::length(dRotation) / UI->deltaTime << "\n";

	if (glm::length(dRotation) < UI->deltaTime * 0.5)
		dRotation = glm::vec3(0);

	this->transform.rotate(dRotation);

	glm::vec3 directionForward = this->transform.getTransformedZ();
	glm::vec3 directionStrafe = this->transform.getTransformedX();
	glm::vec3 directionVertical = this->transform.getTransformedY();

	if (noClip)
	{
		this->velocity = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
	}
	else
	{
		this->velocity = (float) forwardSpeed * directionForward;
	}
	//if (NewEntity<T>::UI->isKeyPressed(GLFW_KEY_L))
	this->transform.position += (float)UI->deltaTime * this->velocity;
}

void PlayerExtension::ballMove()
{
	bool w = UI->isKeyPressed(GLFW_KEY_W);
	bool s = UI->isKeyPressed(GLFW_KEY_S);
	bool a = UI->isKeyPressed(GLFW_KEY_A);
	bool d = UI->isKeyPressed(GLFW_KEY_D);

	bool space = UI->isKeyPressed(GLFW_KEY_SPACE);
	bool ctrl = UI->isKeyPressed(GLFW_KEY_LEFT_CONTROL);

	bool up = UI->isKeyPressed(GLFW_KEY_UP);
	bool down = UI->isKeyPressed(GLFW_KEY_DOWN);
	bool right = UI->isKeyPressed(GLFW_KEY_RIGHT);
	bool left = UI->isKeyPressed(GLFW_KEY_LEFT);
	bool e = UI->isKeyPressed(GLFW_KEY_E);
	bool q = UI->isKeyPressed(GLFW_KEY_Q);

	double forwardSpeed = 0.0;
	double strafeSpeed = 0.0;
	double verticalSpeed = 0.0;

	//MOVEMENT
	if (w == s)
		forwardSpeed = 0.0;
	else if (w)
		forwardSpeed = 10.0;
	else
		forwardSpeed = -10.0;

	if (a == d)
		strafeSpeed = 0.0;
	else if (a)
		strafeSpeed = 10.0;
	else
		strafeSpeed = -10.0;

	if (space == ctrl)
		verticalSpeed = 0.0;
	else if (ctrl)
		verticalSpeed = -10.0;
	else
		verticalSpeed = 10.0;

	if (UI->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		forwardSpeed *= 2.0;
		strafeSpeed *= 2.0;
		verticalSpeed *= 2.0;
	}

	//LOOK
	if (UI->getMouseLockState()) {
		double deltaPitch = UI->deltaMouseY * Settings::mouseSensitivity;
		double deltaPivot = -UI->deltaMouseX * Settings::mouseSensitivity;
		lookRotation.x += deltaPitch;
		lookRotation.y += deltaPivot;
		if (lookRotation.x > 89.9) lookRotation.x = 89.9;
		if (lookRotation.x < -89.9) lookRotation.x = -89.9;
	}

	transform.setRotation(lookRotation);

	glm::vec3 directionForward = this->transform.getTransformedZ();
	glm::vec3 directionStrafe = this->transform.getTransformedX();
	glm::vec3 directionVertical = this->transform.getTransformedY();

	this->velocity = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
	this->transform.position += (float) UI->deltaTime * this->velocity;
}

// ================================= Laser =====================

Laser::Laser(UIManager* UI) : NewEntity(UI)
{

}

void Laser::move()
{
	glm::vec3 directionForward = this->transform.getTransformedZ();
	this->velocity = (float) speed * directionForward;
	this->transform.position += (float)UI->deltaTime * this->velocity;
}

