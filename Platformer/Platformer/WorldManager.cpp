#include "WorldManager.h"

WorldManager::WorldManager()
{
}

WorldManager::~WorldManager()
{
}

std::unique_ptr<b2World> WorldManager::generateWorld(const std::string levelPath, AudioManager& audioManager, 
	Player& player, std::vector<Ground>& groundEntities, std::vector<Box>& boxEntities)
{
	log("Generating world");

	//The box2D world
	std::unique_ptr<b2World> world;

	//Box2D world setup
	b2Vec2 gravity(0.0f, -9.8f);
	world = std::make_unique<b2World>(gravity);

	LevelManager::loadLevel(levelPath, world, audioManager, player, groundEntities, boxEntities);

	log("World generated");

	return world;
}

//Add a ground entity to the world
void WorldManager::addGroundToWorld(std::vector<Ground>& groundEntities, std::unique_ptr<b2World>& world, const glm::vec2 & position, const glm::vec2 & dimensions, const Colour & colour, const Texture & texture, const float friction, const glm::vec4 & texCoords, const bool fixedRotation)
{
	Ground ground;
	ground.init(world.get(), position, dimensions, colour, texture, friction, texCoords, fixedRotation);
	groundEntities.push_back(ground);

	log("Created ground at " + std::to_string(position.x) + " " + std::to_string(position.y));
}

//Add a box entity to the world
void WorldManager::addBoxToWorld(std::vector<Box>& boxEntities, std::unique_ptr<b2World>& world, const glm::vec2 & position, const glm::vec2 & dimensions, const Colour & colour, const Texture & texture, const float density, const float friction, const glm::vec4 & texCoords, const bool fixedRotation)
{
	Box newBox;
	newBox.init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, fixedRotation);
	boxEntities.push_back(newBox);

	log("Created a box at " + std::to_string(position.x) + " " + std::to_string(position.y));
}

//Quickly generates random box data and writes it to a file
//Used for generating random boxes for the level, not actually used in the engine
void WorldManager::generateBoxeData(int n)
{
	//Random box gen
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xGen(10.0f, 30.0f);
	std::uniform_real_distribution<float> yGen(5.0f, 20.0f);
	std::uniform_real_distribution<float> sizeGen(0.5f, 2.5f);
	std::uniform_int_distribution<int> colourGen(200, 255);
	std::uniform_int_distribution<int> textureGen(1, 10);

	//Number of boxes to generate
	float density = 1.0f;
	float friction = 0.3f;

	//File to write to
	std::ofstream file("../res/randomGen/randomBoxes.txt");

	for (int i = 0; i < n; i++)
	{
		//Setup random box params (position, dimensions, colour, texture)
		glm::vec2 position = glm::vec2(xGen(randGenerator), yGen(randGenerator));
		glm::vec2 dimensions = glm::vec2(sizeGen(randGenerator), sizeGen(randGenerator));
		glm::vec4 colour(colourGen(randGenerator), colourGen(randGenerator), colourGen(randGenerator), 255);
		std::string textureString = "../res/textures/boxes_and_crates/obj_box" + std::to_string(textureGen(randGenerator)) + ".png";
		Texture texture = ResourceManager::getTexture(textureString);
		glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

		//Write to file
		file << position.x << ' ' << position.y << ' ' << dimensions.x << ' ' << dimensions.y << ' '
			<< colour.x << ' ' << colour.y << ' ' << colour.z << ' ' << colour.w << ' '
			<< density << ' ' << friction << ' ' << textureString << '\n';
	}
}

void WorldManager::log(const std::string text)
{
	std::cout << "[WorldManager] " << text << std::endl;
}