#include "GameManager.h"

int main(int argc, char* args[])
{
	GameManager gm = GameManager();

	gm.init();
	//gm.loadMedia();
	gm.gameLoop();

	return 0;
}
