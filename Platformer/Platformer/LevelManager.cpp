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

	int x;
	file >> x;

	std::cout << x;

	//Initialise player params
	glm::vec2 playerPos(0.0f, 5.0f);
	glm::vec2 playerDims(1.0f, 2.0f);
	Colour playerColour(255, 255, 255, 255);
	Texture playerTextures[NUM_STATES];
	playerTextures[IDLE] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/idle.png");
	playerTextures[RUN] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/run.png");
	playerTextures[JUMP] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/jump.png");
	playerTextures[IN_AIR] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/jump.png");
	playerTextures[ATTACK] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/attack.png");
	playerTextures[JUMP_ATTACK] = ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/jump_attack.png");
	glm::vec4 playerTexCoords(0.0f, 0.0f, 1.0f, 1.0f);

	SoundEffect playerSounds[NUM_SOUNDS];
	playerSounds[JUMP_SOUND] = audioManager.loadSoundEffect("../res/sound/platformer_jumping/jump_01.wav");
	playerSounds[ATTACK_SOUND] = audioManager.loadSoundEffect("../res/sound/melee_sounds/sword_sound.wav");


	//glm::vec2 playerPos;
	//glm::vec2 playerDims;
	//glm::vec2 playerColour;
	//Texture playerTexture[NUM_STATES];

	//Initialise player instance
	player.init(world.get(), playerPos, playerDims, playerColour, playerTextures, playerTexCoords, playerSounds, true);
}

void LevelManager::log(const std::string text)
{
	std::cout << "[LevelManager] " << text << std::endl;
}