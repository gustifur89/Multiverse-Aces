#pragma once
#include <Engine.hpp>

class Joystick
{
private:
	int buttonCount, axisCount, hatCount;
	int joystickNumber;

	//float* axes;
//	unsigned char* buttons;
//	unsigned char* hats;

public:
	Joystick(int joystickNumber);
	~Joystick();

	bool isPresent();

	int getButtonCount();
	int getAxisCount();
	int getHatCount();

	bool getButton(int buttonNumber);
	float getAxis(int axisNumber);

	static std::shared_ptr<Joystick> getJoystick(int joystickNumber);

};

