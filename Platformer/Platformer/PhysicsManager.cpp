#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::initPhysics(int desiredFPS, std::unique_ptr<b2World>& world, std::vector<Entity>& entities, Player& player)
{
	//Initialisation flag
	bool success = true;

	mBox2DTimeStep = 1.0f / desiredFPS;

	//Box2D world setup
	b2Vec2 gravity(0.0f, -9.8f);
	world = std::make_unique<b2World>(gravity);

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -30.0f);
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Init player
	glm::vec2 playerPos(0.0f, 30.0f);
	glm::vec2 playerDims(1.0f, 2.0f);
	Colour playerColour(255, 255, 255, 255);
	Texture playerTexture = ResourceManager::getTexture("../res/textures/ninja_adventure/png/Idle__000.png");
	glm::vec4 playerTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

	player.init(world.get(), playerPos, playerDims, playerColour, playerTexture, playerTexCoords, true);

	//Random box gen
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xGen(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yGen(0.0f, 30.0f);
	std::uniform_real_distribution<float> sizeGen(0.5f, 2.5f);
	std::uniform_int_distribution<int> colourGen(150, 255);
	std::uniform_int_distribution<int> textureGen(0, 9);
	Texture boxTexture = ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_crate002.png");

	const int NUM_BOXES = 100;

	for (unsigned int i = 0; i < NUM_BOXES; i++)
	{
		float xPos = xGen(randGenerator);
		float yPos = yGen(randGenerator);
		float sizeX = sizeGen(randGenerator);
		float sizeY = sizeGen(randGenerator);
		Colour colour(colourGen(randGenerator), colourGen(randGenerator), colourGen(randGenerator), 255);
		glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

		Box newBox;
		newBox.init(world.get(), glm::vec2(xPos, yPos), glm::vec2(sizeX, sizeY), colour, boxTexture);
		entities.push_back(newBox);
	}

	return success;
}

//Update the world
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world, std::vector<Entity>& entities, Player& player)
{
	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);

	//for (unsigned int i = 0; i < bullets.size();)
	//{
	//	if (bullets[i].update(timeStep) == true) 
	//	{
	//		bullets[i] = bullets.back();
	//		bullets.pop_back();
	//	}
	//	else
	//	{
	//		i++;
	//	}
	//}
}

void PhysicsManager::addBoxToWorld(std::unique_ptr<b2World>& b2World, std::vector<Entity>& entities)
{
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
