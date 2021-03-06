#include "WorldManager.h"

WorldManager::WorldManager()
{
}

WorldManager::~WorldManager()
{
}

void WorldManager::generateWorld(const std::string levelPath, AudioManager& audioManager, 
	Player* player, std::vector<Ground*>& groundEntities, std::vector<Box*>& boxEntities, std::vector<Enemy*>& enemyEntities,
	std::vector<Marker*>& markerEntities, std::vector<glm::vec2>& enemySpawnPositions)
{
	log("Generating world");

	//Box2D world setup
	world = std::make_unique<b2World>(mGravity);

	LevelManager::loadLevel(levelPath, world, audioManager, player, groundEntities, boxEntities, enemyEntities, markerEntities, enemySpawnPositions);

	log("World generated");
}

//Used to ground to the world after the level has been loaded (currently on right click)
void WorldManager::addGroundToWorld(std::vector<Ground*>& groundEntities, std::unique_ptr<b2World>& world, const glm::vec2& position, 
	const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float friction, const glm::vec4& texCoords, 
	const bool fixedRotation)
{
	Ground* ground = new Ground;
	ground->init(world.get(), position, dimensions, colour, texture, friction, texCoords, fixedRotation);
	groundEntities.emplace_back(ground);

	log("Created ground at " + std::to_string(position.x) + " " + std::to_string(position.y));
}

//Used to add boxes to the world after the level has been loaded (currently on left click)
void WorldManager::addBoxToWorld(std::vector<Box*>& boxEntities, std::unique_ptr<b2World>& world, const glm::vec2& position, 
	const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float density, const float friction, 
	const glm::vec4& texCoords, const bool fixedRotation)
{
	Box* newBox = new Box;
	newBox->init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, fixedRotation);
	boxEntities.emplace_back(newBox);

	log("Created a box at " + std::to_string(position.x) + " " + std::to_string(position.y));
}

void WorldManager::log(const std::string text)
{
	std::cout << "[WorldManager] " << text << std::endl;
}