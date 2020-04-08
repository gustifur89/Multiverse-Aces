#include <Engine.hpp>
#include "GameObjectHSF.h"
#include "PlayerExtension.h"
#include "CameraFollower.h"
#include "Joystick.h"
#include "Ship.h"
#include "NewTerrain.h"

int main()
{
	UIManager UI;
	//UI_SSAO
	//900 650
	//900/650
	if (!UI.create(1080, 960, "nihilnauts - Multiverse Aces", 30, UI_SSAO, "windowFragment2"))
	{
		std::cout << "failed to create UI\n";
		return -1;
	}
	glm::vec3 clearColor= 255.f * glm::vec3(0.66, 0.4, 0.98); // red 0.02, 0.33, 0.60 light blue 0.66, 0.1, 0.98 yellow 0.13, 0.26, 0.9 blue 0.56, 0.5, 0.98
	UI.setClearColor(clearColor.x, clearColor.y, clearColor.z);

	srand(0);

	if (!glfwGetCurrentContext())
		std::cout << "No Valid Context\n"; 

	Audio::init();

	std::shared_ptr<GameObject> stage = UI.stage;
	std::shared_ptr<GameObject> world1 = std::shared_ptr<GameObject>(new GameObject());
	std::shared_ptr<GameObject> world2 = std::shared_ptr<GameObject>(new GameObject());
	stage->addChild(world1);
	stage->addChild(world2);

	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Holdable>> items;

	std::shared_ptr<ColorShader> colorShader = ColorShader::loadShader("color");
	std::shared_ptr<HSFShader> hsfShader = HSFShader::loadShader("HSF");

	std::shared_ptr<ColorMesh> cubeMesh = ColorMesh::loadFromFile("DefaultCube");
	std::shared_ptr<ColorMesh> ballMesh = ColorMesh::loadFromFile("DefaultSphere");
	std::shared_ptr<ColorMesh> crystalMesh = ColorMesh::loadFromFile("crystal");
	std::shared_ptr<ColorMesh> pillarMesh = ColorMesh::loadFromFile("x-wing");//pillar

	std::shared_ptr<HSFMesh> xwingMesh = HSFMesh::loadFromFile("x-wing");
	std::shared_ptr<HSFMesh> stardestroyerMesh = HSFMesh::loadFromFile("star destroyer");
	std::shared_ptr<HSFMesh> voidCraftMesh = HSFMesh::loadFromFile("spaceShip");
	std::shared_ptr<HSFMesh> hsfCubeMesh = HSFMesh::loadFromFile("defaultCube");
	std::shared_ptr<HSFMesh> hsfBallMesh = HSFMesh::loadFromFile("DefaultSphere");
	std::shared_ptr<HSFMesh> hsfSmoothSphereMesh = HSFMesh::loadFromFile("SmoothSphere");
//	std::shared_ptr<HSFMesh> areaMesh = HSFMesh::loadFromFile("area_model");
//	std::shared_ptr<HSFMesh> areaBigMesh = HSFMesh::loadFromFile("area_big");
	std::shared_ptr<HSFMesh> laserMesh = HSFMesh::loadFromFile("laser");
	//laserMesh->colorPalette->hsList[0] = 0.40;
	
	std::shared_ptr<Sound> sound0 = Sound::loadFromFile("buzz");
	std::shared_ptr<Sound> pewSND = Sound::loadFromFile("pew");//pewSND
	std::shared_ptr<Sound> engineSND = Sound::loadFromFile("engine");

	std::shared_ptr<SoundEmitter> soundEmmiter0 = std::shared_ptr<SoundEmitter>(new SoundEmitter(pewSND));
	soundEmmiter0->setAttenuation(0.5);
	soundEmmiter0->setMinDistance(15.0);
	soundEmmiter0->setLoop(false);

	std::shared_ptr<SoundEmitter> soundEmmiterEngine = std::shared_ptr<SoundEmitter>(new SoundEmitter(sound0));
	soundEmmiterEngine->setAttenuation(0.5);
	soundEmmiterEngine->setMinDistance(15.0);
	//soundEmmiterEngine->setVolume(0.2);
	soundEmmiterEngine->setLoop(true);

	std::shared_ptr<ColorPalette> colorPalette = std::shared_ptr<ColorPalette>(new ColorPalette);
	colorPalette->hsList = xwingMesh->colorPalette->hsList;

	std::shared_ptr<GameObjectHSF> xwing(new GameObjectHSF);
	xwing->transform.position = glm::vec3(0, -7, 0);
	xwing->mesh = xwingMesh;
	xwing->shader = hsfShader;
	xwing->setColorPalette(colorPalette);// xwingMesh->colorPalette;
	world1->addChild(xwing);


	std::shared_ptr<GameObjectHSF> smoothSphere(new GameObjectHSF);
	smoothSphere->transform.position = glm::vec3(0, 0, 0);
	smoothSphere->mesh = hsfSmoothSphereMesh;
	smoothSphere->shader = hsfShader;
	smoothSphere->setColorPalette(hsfSmoothSphereMesh->colorPalette);// xwingMesh->colorPalette;
	world1->addChild(smoothSphere);

	std::shared_ptr<GameObjectHSF> starDestroyer(new GameObjectHSF);
	starDestroyer->transform.position = glm::vec3(12, 120, 40);
	starDestroyer->transform.setRotation(4, 60, 7);
	starDestroyer->transform.scale = glm::vec3(10, 10, 10);
	starDestroyer->mesh = voidCraftMesh;
	starDestroyer->shader = hsfShader;
	starDestroyer->setColorPalette(voidCraftMesh->colorPalette);
	world1->addChild(starDestroyer);

	/*std::shared_ptr<GameObjectHSF> land(new GameObjectHSF);
	land->transform.position = glm::vec3(0, -20, 0);
	land->transform.scale = glm::vec3(3, 3, 3);
	land->mesh = areaBigMesh;
	land->shader = hsfShader;
	land->setColorPalette(areaBigMesh->colorPalette);
	world1->addChild(land); */


	for (int i = 0; i < 6; i++)
	{
		std::shared_ptr<GameObjectHSF> obj(new GameObjectHSF);
		obj->transform.position = glm::vec3(rand() % 240 - 120, rand() % 240 - 120, rand() % 240 - 120);
		obj->transform.setRotation((rand() % 360), (rand() % 360), (rand() % 360));
		obj->mesh = xwingMesh;
		obj->shader = hsfShader;
		obj->setColorPalette(colorPalette);
		world1->addChild(obj);
	}

	Camera camera(90, UI.aspectRatio, 0.1f, 1000.0f);
	camera.gamma = 2.2;//2.2
	camera.exposure = 0.2;//0.35


	// ship
	std::shared_ptr<HSFMesh> xwingBaseMesh = HSFMesh::loadFromFile("xwing_base");
	std::shared_ptr<HSFMesh> xwingWingMesh = HSFMesh::loadFromFile("xwing_wing");
	std::shared_ptr<HSFMesh> junkerMesh = HSFMesh::loadFromFile("junker_0");

	std::shared_ptr<Ship> player(new Ship(&UI));
	player->setColorPalette(xwingBaseMesh->colorPalette);
	player->shader = hsfShader;
	world1->addChild(player);

	std::shared_ptr<ShipPart> shipBase = std::shared_ptr<ShipPart>(new ShipPart);
	shipBase->mesh = xwingBaseMesh;
	shipBase->setColorPalette(xwingBaseMesh->colorPalette);
	//shipBase->mesh = junkerMesh;
	//shipBase->setColorPalette(junkerMesh->colorPalette);
	player->addPart(shipBase);

	
	std::vector<Transform> wings;
	wings.push_back(Transform(glm::vec3( 0.897,  0.814,  0.463), glm::vec3(0, 0,-10), glm::vec3( 1,  1,  1)));
	wings.push_back(Transform(glm::vec3(-0.897,  0.814,  0.463), glm::vec3(0, 0, 10), glm::vec3(-1, 1,  1)));
	wings.push_back(Transform(glm::vec3( 0.897, -0.514,  0.463), glm::vec3(0, 0, 10), glm::vec3( 1, -1,  1)));
	wings.push_back(Transform(glm::vec3(-0.897, -0.514,  0.463), glm::vec3(0, 0,-10), glm::vec3(-1, -1,  1)));

	for (int i = 0; i < wings.size(); i++)
	{
		std::shared_ptr<ShipPart> shipWing = std::shared_ptr<ShipPart>(new ShipPart);
		shipWing->mesh = xwingWingMesh;
		shipWing->setColorPalette(xwingWingMesh->colorPalette);
		shipWing->transform = wings[i];
		player->addPart(shipWing);
	}
	Light::globalLightDirection = glm::normalize(glm::vec3(0.4, 1, 0.5));
	Light::globalLightIntensity = 3.0;
	Light::ambient = 0.4;

	std::shared_ptr<CameraFollower> camFollower = std::shared_ptr<CameraFollower>(new CameraFollower());
	camFollower->setTarget(player);


	std::shared_ptr<PlayerExtension> camRig(new PlayerExtension(&UI));
	camRig->mesh = hsfBallMesh;
	camRig->shader = hsfShader;
	camRig->setColorPalette(hsfBallMesh->colorPalette);
	world1->addChild(camRig);
	
	std::shared_ptr<Light> light0 = Light::createPointLight(glm::vec3(0, 20, 0), 60.0, 50.0, glm::vec3(255, 255, 255));//Light::createSpotLight(glm::vec3(0,15,0), glm::vec3(0, 0, 0), 30.0, 20.0);
	UI.addLight(light0);
	std::shared_ptr<Light> light1 = Light::createPointLight(glm::vec3(0, 40, 0), 120, 10.0, glm::vec3(255, 255, 255));//Light::createSpotLight(glm::vec3(0,15,0), glm::vec3(0, 0, 0), 30.0, 20.0);
	UI.addLight(light1);


	Toggle mouseLockToggle;
	Toggle updateToggleB;
	Toggle playerToggle;
	bool playerState = true;

	Toggle exposureToggle;
	Toggle gammaToggle;
	Toggle landGenerationToggle;
	Toggle gunToggle;
	bool landGeneration = false;
	bool exposureSweep = false;
	bool gammaSweep = false;

	std::shared_ptr<Joystick> joystick = Joystick::getJoystick(GLFW_JOYSTICK_1);
	double sps = 2.2;
	double gunTime = 0.0;

	/*
	std::shared_ptr<Terrain> land(new Terrain(1024, glm::vec3(40, 40, 40), 20, 0.5, 2.0, glm::vec3(1.0, 1.0, 1.0), 0.002, 2));
	land->shader = colorShader; 
	land->persistentVisible = true;
	world1->addChild(land);
	*/

	/*
	std::shared_ptr<Terrain> terra = std::shared_ptr<Terrain>(new Terrain(1024, glm::vec3(40, 40, 40), 20, 0.25, 2.0, glm::vec3(1.0, 1.0, 1.0), 0.0, 10)); ;// (new Terrain(1024, glm::vec3(20, 20, 40), 20, 2.0, 1.0, glm::vec3(1), 0.0, 1));
	terra->flatTop = true;
	std::shared_ptr<Entity> dummyEnt = std::shared_ptr<Entity>(new Entity(1.0, NULL, &UI));
	terra->shader = colorShader;
	terra->setTarget(dummyEnt); 
	*/

	std::shared_ptr<NewTerrain> terra = std::shared_ptr<NewTerrain>(new NewTerrain);
	terra->setTargetEntity(player);
	terra->shader = colorShader;
	world1->addChild(terra);

	std::vector<std::shared_ptr<Laser>> laserList;

	std::cout << "preRender Errors:\n";
	Gen::checkGLError();
	std::cout << "render Errors:\n";

	bool mkOverride = true;

	do
	{
		if (exposureToggle.toggle(UI.isKeyPressed(GLFW_KEY_9)))
		{
			camera.exposure += 0.05;// *UI.deltaTime;
			if (camera.exposure > 5.0)
			{
				camera.exposure = -1.6;
			}
			std::cout << camera.exposure << "\n";;
		}//exposureSweep = !exposureSweep;
		if (gammaToggle.toggle(UI.isKeyPressed(GLFW_KEY_0)))
		{
			camera.gamma += 0.05;// *UI.deltaTime;
			if (camera.gamma > 3.0)
			{
				camera.gamma = -1.6;
			}
			std::cout << camera.gamma << "\n";;
		}//exposureSweep = !exposureSweep;	gammaSweep = !gammaSweep;

		if (exposureSweep)
		{
			camera.exposure += 0.5 * UI.deltaTime;
			if (camera.exposure > 10.0)
			{
				camera.exposure = -1.0;
			}
		}
		if (gammaSweep)
		{
			camera.gamma += 0.5 * UI.deltaTime;
			if (camera.gamma > 10.0)
			{
				camera.gamma = -1.0;
			}
		} 

		if (landGenerationToggle.toggle(UI.isKeyPressed(GLFW_KEY_5)))
			landGeneration = !landGeneration;
		if (landGeneration)
		{
			terra->update();
		}


		if (playerToggle.toggle(UI.isKeyPressed(GLFW_KEY_4)))
		{
			playerState = !playerState;
			if (playerState)
			{
				terra->setTargetEntity(player);
			}
			else
			{
				terra->setTargetEntity(camRig);
			}
		}

		if (playerState)
		{
			//player->move();
			if (joystick->isPresent() || !mkOverride)
				player->move(joystick);
			else
				player->mouseMove();

			camFollower->move(UI.deltaTime);
			camFollower->updateCamera(camera);
		//	dummyEnt->setPosition(player->transform.position);
			
			soundEmmiter0->transform.position = player->transform.position;
			soundEmmiterEngine->transform.position = player->transform.position;
			double engineNoise = (glm::length(player->velocity) / 60.0);
			soundEmmiterEngine->setVolume(engineNoise);
			soundEmmiterEngine->play();

			if (joystick->isPresent())
			{
				//if (gunToggle.toggle(joystick->getButton(0)))
				if (joystick->getButton(0) && gunTime >= 1.0 / sps)
				{
					gunTime = 0.0;
					//pew pew
					std::vector<glm::vec4> gunPos;
					
					gunPos.push_back(glm::vec4(-9.02,  2.26, 6, 1));
					gunPos.push_back(glm::vec4(-9.02, -1.96, 6, 1));
					gunPos.push_back(glm::vec4( 9.02,  2.26, 6, 1));
					gunPos.push_back(glm::vec4( 9.02, -1.96, 6, 1));
					
					/*
					gunPos.push_back(glm::vec4(0.796, -0.773, 3.756, 1));
					gunPos.push_back(glm::vec4(-0.796, -0.773, -3.756, 1));
					*/
					for (int i = 0; i < gunPos.size(); i++)
					{
						std::shared_ptr<Laser> las = std::shared_ptr<Laser>(new Laser(&UI));
						las->mesh = laserMesh;
						las->transform.scale.z = 4;
						las->shader = hsfShader;
						las->transform.position = glm::vec3(player->transform.getTransform() * gunPos[i]);
						las->transform.setRotation(player->transform);
						las->setColorPalette(laserMesh->colorPalette);
						las->speed = 400;
						laserList.push_back(las);
						world1->addChild(las);
					}
					soundEmmiter0->play();
				}
			}
			else
			{
				//if (gunToggle.toggle(joystick->getButton(0)))
				if (UI.isMouseDown(GLFW_MOUSE_BUTTON_1) && gunTime >= 1.0 / sps)
				{
					gunTime = 0.0;
					//pew pew
					std::vector<glm::vec4> gunPos;

					gunPos.push_back(glm::vec4(-9.02, 2.26, 6, 1));
					gunPos.push_back(glm::vec4(-9.02, -1.96, 6, 1));
					gunPos.push_back(glm::vec4(9.02, 2.26, 6, 1));
					gunPos.push_back(glm::vec4(9.02, -1.96, 6, 1));

					/*
					gunPos.push_back(glm::vec4(0.796, -0.773, 3.756, 1));
					gunPos.push_back(glm::vec4(-0.796, -0.773, -3.756, 1));
					*/
					for (int i = 0; i < gunPos.size(); i++)
					{
						std::shared_ptr<Laser> las = std::shared_ptr<Laser>(new Laser(&UI));
						las->mesh = laserMesh;
						las->transform.scale.z = 4;
						las->shader = hsfShader;
						las->transform.position = glm::vec3(player->transform.getTransform() * gunPos[i]);
						las->transform.setRotation(player->transform);
						las->setColorPalette(laserMesh->colorPalette);
						las->speed = 400;
						laserList.push_back(las);
						world1->addChild(las);
					}
					soundEmmiter0->play();
				}
			}
			gunTime += UI.deltaTime;
		}
		else
		{
			camRig->ballMove();
			camRig->updateCamera(camera);
			//light0->position = camRig->transform.position;
		}


	//	Light::globalLightDirection = glm::vec3(glm::mat4(glm::quat(glm::radians(glm::vec3(0,1,0))))*glm::vec4(Light::globalLightDirection, 1));// glm::normalize(glm::vec3(0.1, 1, 0.1));

		if (mouseLockToggle.toggle(UI.isKeyPressed(GLFW_KEY_2)))
		{
			if (UI.getMouseLockState())
				UI.unlockMouse();
			else
				UI.lockMouse();
		}

		colorPalette->hsList[2] += 0.4 * UI.deltaTime;
		if (colorPalette->hsList[2] > 1.0) colorPalette->hsList[2] -= 1.0;

	//	color = 255.f * glm::vec3(colorPalette->hsList[2], 1.0, 1.0);
	////	UI.setClearColor(color.x, color.y, color.z);
	//	glm::vec3 color = 255.f * glm::vec3(colorPalette->hsList[2], 0.2, 0.4);
	//	UI.setClearColor(color.x, color.y, color.z);
	//	light0->position = camRig->transform.position;

		for (std::shared_ptr<Laser> laser : laserList)
		{
			laser->move();
		}

		Audio::configureListener(camera.position, camera.getEulerAngles());
		UI.display(camera, stage);
	//	Gen::checkGLError();
	} while (UI.isWindowOpen());

	UI.delay(0.2);

	return 1;
}
