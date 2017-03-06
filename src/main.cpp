#include "Game.h"
#include "SDLWrapper.h"
#include "ViewModel.h"
#include <iostream>

int main(int argc, char *argv[])
{
	try
	{
		SDLWrapper graphics;
		Game game;
		ViewModel viewModel(&game,&graphics);
		while (!game.getHasEnded())
		{
			viewModel.handleEvents();
			game.gameLoop();
			viewModel.drawLoop();
		}
		return 0;
	}
	catch (const InitError& err)
	{
		std::cerr
			<< "Error while initializing SDL: "
			<< err.what() << std::endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            "Initialization error",
            err.what(),
            NULL);
	}
	catch (const std::bad_alloc& err)
	{
		std::cerr
			<< "Error while allocating memory: "
			<< err.what() << std::endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            "Memory error",
            err.what(),
            NULL);
	}
	return 1;
}
