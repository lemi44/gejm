#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <SDL_stdinc.h>

//#define DEBUGGAME /* Uncomment to add debug info */

/**
 * Hardcoded constants.
 * Replace with information loaded from file ASAP!
 */
namespace GameDefinitions //TODO: Load informations from file
{
	//Game strings
	char const *const gameTitle = "GEJM";
    char const *const gameSubtitles[] =
    { "or: Why we regret we decided to make a platformer",
        "a simple platformer made by poor students",
        "made by Michal Lesniak and Dominik Paluch" };
    int const gameSubtitlesSize = 3;
    char const *const gameLost = "YOU DIED";
    char const *const gameWon = "YOU WIN";

	//Screen dimension constants
	const int screenWidth = 640;
	const int screenHeight = 480;
	const int scale = screenHeight/16; // 30 for 480
}
#endif // GAMEDEFS_H