#include "LevelManager.h"

bool LevelManager::loadLevel(const std::string& filePath, std::unique_ptr<b2World>& world, 
	AudioManager& audioManager, Player& player, std::vector<Ground>& groundEntities,
	std::vector<Box>& boxEntities)
{
	bool success = true;

	log("Loading level: " + filePath);

	std::ifstream file(filePath);
	if (file.fail())
	{
		log("Failed to load level " + filePath);
		return success = false;
	}


	{
		//Load the ground entities
		log("Loading ground");

		int n;					//number of ground entities taken from file
		file >> n;

		glm::vec2 position;
		glm::vec2 dimensions;
		Colour colour;
		glm::vec4 colourVec;

		Texture texture;
		float friction;

		//Load each ground entity
		for (int i = 0; i < n; i++)
		{
			file >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
				colourVec.z >> colourVec.w >> friction;
			colour = Colour(colourVec);

			std::string path;
			file >> path;
			texture = ResourceManager::getTexture(path);
			glm::vec4 texCoords = { position.x, position.y, dimensions.x, dimensions.y };

			Ground ground;
			ground.init(world.get(), position, dimensions, colour, texture, friction, texCoords, true);
			groundEntities.push_back(ground);
		}

		log("Ground loaded");
	}

	{
		//Load the box entities
		log("Loading boxes");

		int n;					//number of box entities taken from file
		file >> n;

		glm::vec2 position;
		glm::vec2 dimensions;
		Colour colour;
		glm::vec4 colourVec;

		Texture texture;
		float density;
		float friction;

		//Load each box entity
		for (int i = 0; i < n; i++)
		{
			file >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
				colourVec.z >> colourVec.w >> density >> friction;
			colour = Colour(colourVec);

			std::string path;
			file >> path;
			texture = ResourceManager::getTexture(path);
			glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

			Box box;
			box.init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, false);
			boxEntities.push_back(box);
		}

		log("Boxes loaded");
	}

	//Load the player entity
	{
		log("Loading player");

		//Initialise player params
		glm::vec2 position;
		glm::vec2 dimensions;
		Colour colour;
		glm::vec4 colourVec;

		std::string test1;
		std::string test2;
		std::string test3;
		std::string test4;
		std::string test5;
		std::string test6;
		std::string test7;

		Texture playerTextures[NUM_STATES];
		SoundEffect playerSounds[NUM_SOUNDS];

		file >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
			colourVec.z >> colourVec.w;
		colour = Colour(colourVec);

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

		log("Player loaded");
	}

	log("Level successfully loaded from: " + filePath);

	return success;
}

void LevelManager::log(const std::string text)
{
	std::cout << "[LevelManager] " << text << std::endl;
}