#include "Joystick.h"

// ============================ Joystick ==================

Joystick::Joystick(int joystickNumber)
{
	this->joystickNumber = joystickNumber;

	const float * axes = glfwGetJoystickAxes(joystickNumber, &axisCount);
	const unsigned char* buttons = glfwGetJoystickButtons(joystickNumber, &buttonCount);
	const unsigned char* hats = glfwGetJoystickHats(joystickNumber, &hatCount);
}

Joystick::~Joystick()
{

}

bool Joystick::isPresent()
{
	return glfwJoystickPresent(joystickNumber) == GLFW_TRUE;
}

int Joystick::getButtonCount()
{
	return buttonCount;
}

int Joystick::getAxisCount()
{
	return axisCount;
}

int Joystick::getHatCount()
{
	return hatCount;
}

bool Joystick::getButton(int buttonNumber)
{
	const unsigned char* buttons = glfwGetJoystickButtons(joystickNumber, &buttonCount);
	if (buttonNumber >= 0 && buttonNumber < buttonCount)
	{
		return buttons[buttonNumber] == GLFW_PRESS;
	}
	else
	{
		return false;
	}
}

float Joystick::getAxis(int axisNumber)
{
	const float* axes = glfwGetJoystickAxes(joystickNumber, &axisCount);
	if (axisNumber >= 0 && axisNumber < axisCount)
	{
		return axes[axisNumber];
	}
	else
	{
		return 0.0;
	}
}

std::shared_ptr<Joystick> Joystick::getJoystick(int joystickNumber)
{
	return std::shared_ptr<Joystick>(new Joystick(joystickNumber));
}
