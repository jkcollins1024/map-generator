#include <Windows.h>
#include "MainGame.h"

int main(int argc, char** argv) {

	MainGame maingame(768, 768);
	maingame.run();

	return 0;
}