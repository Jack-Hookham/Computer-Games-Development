#include "LevelManager.h"

bool LevelManager::loadLevel(const std::string filePath, std::unique_ptr<b2World>& world, 
	AudioManager& audioManager, Player* player, std::vector<Ground*>& groundEntities,
	std::vector<Box*>& boxEntities, std::vector<Enemy*>& enemyEntities, std::vector<Marker*>& markerEntities, 
	std::vector<glm::vec2>& enemySpawnPositions)
{
	bool success = true;

	log("Loading level: " + filePath);

	std::ifstream file;
	file.open(filePath);
	if (file.fail())
	{
		log("Failed to load level " + filePath);
		return success = false;
	}

	//Store the current line
	std::string line;

	//Check that the file is a valid level file
	std::getline(file, line);
	if (line != "JackH Level")
	{
		log(filePath + " not a valid level file");
		return success = false;
	}

	//Search the file
	while (!file.eof())
	{
		std::getline(file, line);
		if (line == "STARTPLAYER")
		{
			//Unlike ground and boxes, player data isn't all on one line in the level file
			//Before passing in the player data to loadPlayer() it all needs to be put on one line
			std::string playerData = "";
			//Get the next line
			std::getline(file, line);
			//Load the box entities
			log("Loading player");
			while (line != "ENDPLAYER")
			{
				//Add the line to playerData
				playerData += line + " ";
				std::getline(file, line);
			}
			loadPlayer(world, player, audioManager, playerData);
			log("Player loaded");
		}

		else if (line == "STARTGROUND")
		{
			//Get the next line
			std::getline(file, line);
			//Load the box entities
			log("Loading ground");
			while (line != "ENDGROUND")
			{
				loadGround(world, groundEntities, line);
				std::getline(file, line);
			}
			log("Ground loaded");

		}

		else if (line == "STARTBOX")
		{
			//Get the next line
			std::getline(file, line);
			//Load the box entities
			log("Loading boxes");
			while (line != "ENDBOX")
			{
				loadBox(world, boxEntities, line);
				std::getline(file, line);
			}
			log("Boxes loaded");
		}

		else if (line == "STARTENEMY")
		{
			//Get the next line
			std::getline(file, line);
			//Load the enemy entities
			log("Loading enemies");
			while (line != "ENDENEMY")
			{
				loadEnemy(world, enemyEntities, audioManager, line);
				std::getline(file, line);
			}
			log("Enemies loaded");
		}

		//Markers used for enemy AI
		else if (line == "STARTMARKER")
		{
			Texture texture = ResourceManager::getTexture("../res/textures/other/marker.png");
			//Get the next line
			std::getline(file, line);
			//Load the marker entities
			log("Loading markers");
			while (line != "ENDMARKER")
			{
				loadMarker(markerEntities, line, texture);
				std::getline(file, line);
			}
			log("Markers loaded");
		}

		else if (line == "STARTSPAWNPOS")
		{
			//Get the next line
			std::getline(file, line);
			//Load the spawn positions
			log("Loading enemy spawn points");
			while (line != "ENDSPAWNPOS")
			{
				addSpawnPos(enemySpawnPositions, line);
				std::getline(file, line);
			}
			log("Spawn points loaded");
		}
	}

	log("Level successfully loaded from: " + filePath);

	file.close();

	return success;
}

void LevelManager::log(const std::string text)
{
	std::cout << "[LevelManager] " << text << std::endl;
}

void LevelManager::loadPlayer(std::unique_ptr<b2World>& world, Player* player, AudioManager& audioManager, const std::string line)
{
	//Initialise player params
	glm::vec2 position;
	glm::vec2 dimensions;
	Colour colour;
	glm::vec4 colourVec;	
	Texture playerTextures[Player::NUM_STATES];
	SoundEffect playerSounds[Player::NUM_SOUNDS];

	std::istringstream iss(line);

	//Populate params from string stream
	iss >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
		colourVec.z >> colourVec.w;

	colour = Colour(colourVec);

	//Populate the texture array
	for (int i = 0; i < Player::NUM_STATES; i++)
	{
		std::string texturePath;
		iss >> texturePath;
		playerTextures[i] = ResourceManager::getTexture(texturePath, false);
	}

	//Populate the sound array
	for (int i = 0; i < Player::NUM_SOUNDS; i++)
	{
		std::string soundPath;
		iss >> soundPath;
		playerSounds[i] = audioManager.loadSoundEffect(soundPath);
	}

	//Initialise player instance
	player->init(world.get(), position, dimensions, colour, playerTextures, playerSounds, true);
}

void LevelManager::loadGround(std::unique_ptr<b2World>& world, std::vector<Ground*>& groundEntities, const std::string line)
{
	//Ground params
	glm::vec2 position;
	glm::vec2 dimensions;
	Colour colour;
	glm::vec4 colourVec;
	float friction;
	std::string texturePath;
	Texture texture;

	std::istringstream iss(line);

	//Populate params from string stream
	iss >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
		colourVec.z >> colourVec.w >> friction >> texturePath;

	colour = Colour(colourVec);
	texture = ResourceManager::getTexture(texturePath);
	glm::vec4 texCoords = { position.x, position.y, dimensions.x, dimensions.y };

	//Create the ground entity
	Ground* ground = new Ground;
	ground->init(world.get(), position, dimensions, colour, texture, friction, texCoords, true);
	groundEntities.emplace_back(ground);

}

void LevelManager::loadBox(std::unique_ptr<b2World>& world, std::vector<Box*>& boxEntities, const std::string line)
{
	//Box params
	glm::vec2 position;
	glm::vec2 dimensions;
	Colour colour;
	glm::vec4 colourVec;
	float density;
	float friction;
	std::string texturePath;
	Texture texture;

	std::istringstream iss(line);

	//Populate params from string stream
	iss >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
		colourVec.z >> colourVec.w >> density >> friction >> texturePath;

	colour = Colour(colourVec);
	texture = ResourceManager::getTexture(texturePath);
	glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

	//Create the box entity
	Box* box = new Box;
	box->init(world.get(), position, dimensions, colour, texture, density, friction, texCoords, false);
	boxEntities.emplace_back(box);
}

void LevelManager::loadEnemy(std::unique_ptr<b2World>& world, std::vector<Enemy*>& enemyEntities, AudioManager& audioManager, 
	const std::string line)
{
	Enemy* enemy = new Enemy;

	//Initialise player params
	glm::vec2 position;
	glm::vec2 dimensions;
	Colour colour;
	glm::vec4 colourVec;
	Texture enemyTextures[Enemy::NUM_STATES];
	SoundEffect enemySounds[Enemy::NUM_SOUNDS];

	std::istringstream iss(line);

	//Populate params from string stream
	iss >> position.x >> position.y >> dimensions.x >> dimensions.y >> colourVec.x >> colourVec.y >>
		colourVec.z >> colourVec.w;

	colour = Colour(colourVec);

	//Populate the texture array
	for (int i = 0; i < Enemy::NUM_STATES; i++)
	{
		std::string texturePath;
		iss >> texturePath;
		enemyTextures[i] = ResourceManager::getTexture(texturePath, false);
	}

	//Populate the sound array
	for (int i = 0; i < Enemy::NUM_SOUNDS; i++)
	{
		std::string soundPath;
		iss >> soundPath;
		enemySounds[i] = audioManager.loadSoundEffect(soundPath);
	}

	//Initialise enemy instance
	enemy->init(world.get(), position, dimensions, colour, enemyTextures, enemySounds, true);
	enemyEntities.emplace_back(enemy);
}

void LevelManager::loadMarker(std::vector<Marker*>& markerEntities, const std::string line, const Texture& texture)
{
	//Box params
	glm::vec2 position;
	glm::vec2 dimensions;
	Colour colour;

	std::istringstream iss(line);

	//Populate params from string stream
	iss >> position.x >> position.y >> dimensions.x >> dimensions.y;

	colour = Colour(255, 255, 255, 128);
	glm::vec4 texCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

	//Create the box entity
	Marker* marker = new Marker;
	marker->init(position, dimensions, colour, texture, texCoords);
	markerEntities.emplace_back(marker);
}

void LevelManager::addSpawnPos(std::vector<glm::vec2>& enemySpawnPositions, const std::string line)
{
	glm::vec2 position;
	std::istringstream iss(line);
	iss >> position.x >> position.y;
	enemySpawnPositions.emplace_back(position);
}

//Quickly generates random box data and writes it to a file
//Used for generating random boxes for the level, not actually used in the engine
void LevelManager::generateBoxeData(int n)
{
	//Random box generation
	std::mt19937 randGenerator;
	//std::mt19937 randGenerator(time(0));
	std::uniform_real_distribution<float> xGen(10.0f, 30.0f);
	std::uniform_real_distribution<float> yGen(5.0f, 20.0f);
	std::uniform_real_distribution<float> sizeGen(0.5f, 2.5f);
	std::uniform_int_distribution<int> colourGen(200, 255);
	std::uniform_int_distribution<int> textureGen(1, 10);

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
