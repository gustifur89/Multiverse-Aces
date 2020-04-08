#pragma once
#include <Engine.hpp>

class CameraFollower : public Transform
{
private:
	std::shared_ptr<GameObject> target;
	double followingDistance;

public:
	CameraFollower();
	~CameraFollower();

	void updateCamera(Camera& camera);

	void move(double deltaTime);


	void setTarget(std::shared_ptr<GameObject> target);
	void setFollowingDistance(double distance);
};

