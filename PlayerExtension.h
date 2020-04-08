#pragma once
#include <Engine.hpp>
#include "GameObjectHSF.h"
#include "Joystick.h"


class NewEntity : public GameObjectHSF
{
public:
	NewEntity(UIManager* UI)
	{
		this->UI = UI;
		velocity = glm::vec3(0);
	}

	~NewEntity()
	{

	}

	void updateCamera(Camera& camera)
	{
		camera.position = this->transform.position;
		camera.setRotation(this->transform);// = this->transform.rotation;
	}

	virtual void move() = 0;

	UIManager* UI;
	glm::vec3 velocity;
};


class PlayerExtension : public NewEntity
{ 
public:
	PlayerExtension(UIManager* UI);

	void move();
	void ballMove();
	void move(std::shared_ptr<Joystick> joystick);

	Toggle noClipToggle;
	bool noClip;
	glm::vec3 lookRotation;
};

class Laser : public NewEntity
{
public:
	Laser(UIManager* UI);

	void move();

	double speed;
};

