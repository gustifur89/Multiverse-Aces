#include "Ship.h"

// ================================ ShipPart ==========================

ShipPart::ShipPart()
{

}

void ShipPart::setShipColorPalette(std::shared_ptr<ColorPalette> colorPalette)
{
	this->shipColorPalette = colorPalette;
}

std::shared_ptr<ColorPalette> ShipPart::getColorPalette()
{
	return this->colorPalette;
}

// ================================ Ship ==============================

Ship::Ship(UIManager* UI) : NewEntity(UI)
{
	noClip = false;
}

void Ship::addPart(std::shared_ptr<ShipPart> part)
{
	this->addChild(part);
	part->shader = this->shader;
	part->setShipColorPalette(this->getColorPalette());
}

void Ship::move()
{

}

void Ship::move(std::shared_ptr<Joystick> joystick)
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

	double speed = 20 * (1.0 - joystick->getAxis(2)) + 10.0;
	double rotationSpeed = 80;//160.0;

	//MOVEMENT
	if (noClip)
	{
		if (w == s)
			forwardSpeed = 0.0;
		else if (w)
			forwardSpeed = speed;
		else
			forwardSpeed = -speed;
	}
	else
	{
		if (w == s)
			forwardSpeed = speed;
		else if (w)
			forwardSpeed = 2.0 * speed;
		else
			forwardSpeed = 0.5 * speed;
	}

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


	dRotation = (float)(UI->deltaTime * rotationSpeed) * dRotation;

	//std::cout << glm::length(dRotation) / UI->deltaTime << "\n";

	if (glm::length(dRotation) < UI->deltaTime * 0.5)
		dRotation = glm::vec3(0);

	//if(forwardSpeed != 0.0)
		this->transform.rotate(dRotation);
	//else if (space)
	//{
	//	this->transform.rotate(dRotation);
	//}
	glm::vec3 directionForward = this->transform.getTransformedZ();
	glm::vec3 directionStrafe = this->transform.getTransformedX();
	glm::vec3 directionVertical = this->transform.getTransformedY();

	if (noClip)
	{
		this->velocity = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
	}
	else
	{
		this->velocity = (float)forwardSpeed * directionForward;
	}
	//if (NewEntity<T>::UI->isKeyPressed(GLFW_KEY_L))
	this->transform.position += (float)UI->deltaTime * this->velocity;
}

void Ship::mouseMove()
{
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
	this->transform.position += (float)UI->deltaTime * this->velocity;
}
