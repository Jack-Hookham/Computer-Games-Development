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

	//Timestep variable for the box2D step function
	mBox2DTimeStep = 1.0f / desiredFPS;

	//Box2D world setup
	b2Vec2 gravity(0.0f, -9.8f);
	world = std::make_unique<b2World>(gravity);

	if (world == NULL)
	{
		success = false;
	}

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
	std::uniform_int_distribution<int> colourGen(200, 255);
	std::uniform_int_distribution<int> textureGen(1, 5);
	std::uniform_int_distribution<int> textureGenX(0, 4);
	std::uniform_int_distribution<int> textureGenY(0, 1);

	//Number of boxes to generate
	const int NUM_BOXES = 100;

	for (unsigned int i = 0; i < NUM_BOXES; i++)
	{
		//Setup random box params (position, dimensions, colour, texture)
		glm::vec2 position = glm::vec2(xGen(randGenerator), yGen(randGenerator));
		glm::vec2 dimensions = glm::vec2(sizeGen(randGenerator), sizeGen(randGenerator));
		Colour colour(colourGen(randGenerator), colourGen(randGenerator), colourGen(randGenerator), 255);
		std::string textureString = "../res/textures/boxes_and_crates/obj_crate00" + std::to_string(textureGen(randGenerator)) + ".png";
		Texture boxTexture = ResourceManager::getTexture(textureString);
		//Texture boxTextures = ResourceManager::getTexture("../res/textures/boxes_and_crates/boxes_sheet.png");
		//float textureX = textureGenX(randGenerator);
		//float textureY = textureGenY(randGenerator);
		//glm::vec4 texCoords = { textureX * 0.2f, textureY * 0.5f,  (textureX + 1) * 0.2f, (textureY + 1) * 0.5f };

		//Add the box
		addBoxToWorld(entities, world, position, dimensions, colour, boxTexture);
	}

	return success;
}

//Update the world
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world, std::vector<Entity>& entities, Player& player)
{
	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::addBoxToWorld(std::vector<Entity>& entities, std::unique_ptr<b2World>& world,
	const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture, 
	const glm::vec4& texCoords)
{
	Box newBox;
	newBox.init(world.get(), position, dimensions, colour, texture, texCoords);
	entities.push_back(newBox);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
