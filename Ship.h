#pragma once
#include <Engine.hpp>
#include "PlayerExtension.h"
class Ship;


class ShipPart : public GameObjectHSF
{
public:
	ShipPart();
	std::shared_ptr<ShipPart> parent;
	std::shared_ptr<Ship> mainShip;
	std::shared_ptr<ColorPalette> shipColorPalette;

	void setShipColorPalette(std::shared_ptr<ColorPalette> colorPalette);

	std::shared_ptr<ColorPalette> getColorPalette();

};


class Ship : public NewEntity
{
public:
	Ship(UIManager* UI);
	std::shared_ptr<ColorPalette> shipColorPallete;
	std::shared_ptr<bool> useShipColorPtr;
	bool useShipColor;

	void addPart(std::shared_ptr<ShipPart> part);

	void move();
	void move(std::shared_ptr<Joystick> joystick);
	void mouseMove();

	Toggle noClipToggle;
	bool noClip;
};

