#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::initPhysics(const int desiredFPS, std::unique_ptr<b2World>& world, Player& player, 
	std::vector<Box>& boxEntities, std::vector<Ground>& groundEntities)
{
	//Initialisation flag
	bool success = true;

	//Timestep variable for the box2D step function
	mBox2DTimeStep = 1.0f / desiredFPS;

	//Box2D world setup
	b2Vec2 gravity(0.0f, -9.8f);
	world = std::make_unique<b2World>(gravity);

	if (world == NULL)
	{
		success = false;
	}

	////Ground body definition
	//b2BodyDef groundBodyDef;
	//groundBodyDef.position.Set(0.0f, -30.0f);
	//b2Body* groundBody = world->CreateBody(&groundBodyDef);

	////Ground fixture definition
	//b2PolygonShape groundBox;
	//groundBox.SetAsBox(50.0f, 10.0f);
	//groundBody->CreateFixture(&groundBox, 0.0f);

	generateGround(world, groundEntities);

	//Initialise player params
	glm::vec2 playerPos(0.0f, 15.0f);
	glm::vec2 playerDims(1.0f, 2.0f);
	Colour playerColour(255, 255, 255, 255);
	Texture playerTexture = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/running.png");
	glm::vec4 playerTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

	//Initialise player instance
	player.init(world.get(), playerPos, playerDims, playerColour, playerTexture, playerTexCoords, true);

	generateBoxes(world, boxEntities);

	return success;
}

//Update the world
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world)
{
	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::generateGround(std::unique_ptr<b2World>& world, std::vector<Ground>& entities)
{
	float width = 50.0f;
	float height = 1.0f;
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 dimensions = glm::vec2(width, height);
	Colour colour(255, 255, 255, 255);
	Texture texture = ResourceManager::getTexture("../res/textures/platformerArt/png/ground.png");
	glm::vec4 texCoords = { 0.0f, 0.0f, width, height };

	addGroundToWorld(entities, world, position, dimensions, colour, texture, 0.0f, 0.3f, texCoords);
}

void PhysicsManager::generateBoxes(std::unique_ptr<b2World>& world, std::vector<Box>& entities)
{
	//Random box gen
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xGen(10.0f, 30.0f);
	std::uniform_real_distribution<float> yGen(5.0f, 20.0f);
	std::uniform_real_distribution<float> sizeGen(0.5f, 2.5f);
	std::uniform_int_distribution<int> colourGen(200, 255);
	std::uniform_int_distribution<int> textureGen(1, 10);

	//Number of boxes to generate
	const int NUM_BOXES = 50;

	for (unsigned int i = 0; i < NUM_BOXES; i++)
	{
		//Setup random box params (position, dimensions, colour, texture)
		glm::vec2 position = glm::vec2(xGen(randGenerator), yGen(randGenerator));
		glm::vec2 dimensions = glm::vec2(sizeGen(randGenerator), sizeGen(randGenerator));
		Colour colour(colourGen(randGenerator), colourGen(randGenerator), colourGen(randGenerator), 255);
		std::string textureString = "../res/textures/boxes_and_crates/obj_box" + std::to_string(textureGen(randGenerator)) + ".png";
		Texture texture = ResourceManager::getTexture(textureString);
		glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

		//Add the box
		addBoxToWorld(entities, world, position, dimensions, colour, texture, 1.0f, 0.3f, texCoords);
	}
}

void PhysicsManager::addGroundToWorld(std::vector<Ground>& entities, std::unique_ptr<b2World>& world, 
	const glm::vec2 & position, const glm::vec2 & dimensions, const Colour & colour, const Texture & texture,
	const float density, const float friction, const glm::vec4 & texCoords, const bool fixedRotation)
{
	Ground ground;
	ground.init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, fixedRotation);
	entities.push_back(ground);
}

void PhysicsManager::addBoxToWorld(std::vector<Box>& entities, std::unique_ptr<b2World>& world,
	const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture, 
	const float density, const float friction, const glm::vec4& texCoords, const bool fixedRotation)
{
	Box newBox;
	newBox.init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, fixedRotation);
	entities.push_back(newBox);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
