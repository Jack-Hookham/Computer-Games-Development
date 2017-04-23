#include "LevelManager.h"

bool LevelManager::loadLevel(const std::string& filePath, std::unique_ptr<b2World>& world, AudioManager& audioManager,
	Player& player, std::vector<Box>& boxEntities, std::vector<Ground>& groundEntities)
{
	bool success = true;

	log("Loading level: " + filePath);

	std::ifstream file(filePath);
	if (file.fail())
	{
		log("Failed to load level " + filePath);
		return success = false;
	}

	//Load the player entity
	log("Loading player");

	//Initialise player params
	glm::vec2 position;
	glm::vec2 dimensions;

	Colour colour;
	float r;
	float g;
	float b;
	float a;

	Texture playerTextures[NUM_STATES];
	SoundEffect playerSounds[NUM_SOUNDS];

	file >> position.x >> position.y >> dimensions.x >> dimensions.y >> r >> g >>
		b >> a;

	colour = Colour(r, g, b, a);

	for (int i = 0; i < NUM_STATES; i++)
	{
		std::string path;
		file >> path;
		playerTextures[i] = ResourceManager::getTexture(path);
	}

	for (int i = 0; i < NUM_SOUNDS; i++)
	{
		std::string path;
		file >> path;
		playerSounds[i] = audioManager.loadSoundEffect(path);
	}

	//Initialise player instance
	player.init(world.get(), position, dimensions, colour, playerTextures, playerSounds, true);


	//Load the ground entities
	log("Loading ground");
	//Initialise
	int groundNum;			//number of ground entities
	file >> groundNum;

	Texture texture;
	float density;
	float friction;

	for (int i = 0; i < groundNum; i++)
	{
		file >> position.x >> position.y >> dimensions.x >> dimensions.y >> r >> g >> b >> a >>
			friction >> density;
		colour = Colour(r, g, b, a);
		std::string path;
		file >> path;
		texture = ResourceManager::getTexture(path);
		glm::vec4 texCoords = { position.x, position.y, dimensions.x, dimensions.y };

		Ground ground;
		ground.init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, true);
		groundEntities.push_back(ground);
	}



	log("Level successfully loaded from: " + filePath);

	//float width = 50.0f;
	//float height = 1.0f;
	//glm::vec2 position = glm::vec2(0.0f, 0.0f);
	//glm::vec2 dimensions = glm::vec2(width, height);
	//Colour colour(255, 255, 255, 255);
	//Texture texture = ResourceManager::getTexture("../res/textures/platformerArt/png/ground.png");
	//glm::vec4 texCoords = { 0.0f, 0.0f, width, height };

	//addGroundToWorld(entities, world, position, dimensions, colour, texture, 0.0f, 0.3f, texCoords);

	return success;
}

void LevelManager::log(const std::string text)
{
	std::cout << "[LevelManager] " << text << std::endl;
}