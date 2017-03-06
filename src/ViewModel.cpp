#include "ViewModel.h"
#include "Coin.h"
#include "Trigger.h"
#include <string>

/**
 * ViewModel implementation
 */

/**
 * Initializes variables.
 * @param game a constant pointer to a Game object
 * @param sdlWrapper a constant pointer to an SDLWrapper
 */
ViewModel::ViewModel(Game* const game, SDLWrapper* const sdlWrapper) :
	game(game),
	sdlWrapper(sdlWrapper),
	hasDrawnPauseMenu(false),
	countedFrames(0),
	otherCountedFrames(0),
	avgFPS(0),
	curFPS(0)
{
    camera = { 0, 0 };
    SDL_GetRendererOutputSize(sdlWrapper->renderer, &camera.w, &camera.h);
	fpsTimer.start();
	curFPSTimer.start();
}

/**
 * Default destructor
 */
ViewModel::~ViewModel()
{
}

/**
 * Handling events.
 * @return void
 */
void ViewModel::handleEvents()
{
	SDL_Event events;
	//Event handling
	while (SDL_PollEvent(&events) != 0)
	{
		switch (events.type)
		{
		case SDL_QUIT:
			game->gameOver();
			break;
		case SDL_KEYDOWN:
			if (events.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				switch (game->getGameState())
				{
				case GameState::Playing:
					game->pauseGame();
					break;
				case GameState::Paused:
					game->resumeGame();
					break;
				}
			}
			break;
		}

		//If SDL_QUIT, ignore events
		if (game->getGameState() == GameState::Quit)
			break;
	}
}

/**
 * A draw loop.
 * Interprets current game state and chooses what to draw.
 * @return void
 */
void ViewModel::drawLoop()
{
	switch (game->getGameState())
	{
	case GameState::Menu:
        fadeTimer.stop();
		drawMenu();
		break;
	case GameState::Playing:
        fadeTimer.stop();
		drawGame();
		hasDrawnPauseMenu = false;
		break;
	case GameState::Paused:
		drawPauseMenu();
		hasDrawnPauseMenu = true;
		break;
    case GameState::Lost:
        drawLost();
        break;
    case GameState::Won:
        drawWon();
        break;
	default:
		break;
	}
#ifdef DEBUGGAME
	currentFPS();
	correctFPS();
	std::string string = "avgFPS:";
	string += std::to_string(avgFPS);
	string += " curFPS:";
	string += std::to_string(curFPS);
	SDL_SetWindowTitle(sdlWrapper->window, string.c_str());
#endif
}

/**
 * Interprets all game objects and draws them accordingly on screen.
 * @return void
 */
void ViewModel::drawGame()
{
    SDL_ShowCursor(0);
	if (game->getPosUpdated())
	{
		//Clear screen
		SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(sdlWrapper->renderer);

		//Center the camera over the player
		camera.x = int((game->getPlayerController()->getCreature()->getX() + game->getPlayerController()->getCreature()->getWidth() / 2)*GameDefinitions::scale - GameDefinitions::screenWidth / 2);
		camera.y = int((game->getPlayerController()->getCreature()->getY() + game->getPlayerController()->getCreature()->getHeight() / 2)*GameDefinitions::scale - GameDefinitions::screenHeight / 2);

		//Keep the camera in bounds
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > game->getLevelWidth()*GameDefinitions::scale - camera.w)
		{
			camera.x = game->getLevelWidth()*GameDefinitions::scale - camera.w;
		}
		if (camera.y > game->getLevelHeight()*GameDefinitions::scale - camera.h)
		{
			camera.y = game->getLevelHeight()*GameDefinitions::scale - camera.h;
		}

		for (std::list<Object*>::const_iterator it = game->getObjectList().begin(); it != game->getObjectList().end(); ++it)
		{
			if ((*it)->getX()*GameDefinitions::scale < camera.x + camera.w &&
				((*it)->getX() + (*it)->getWidth())*GameDefinitions::scale > camera.x &&
				(*it)->getY()*GameDefinitions::scale < camera.y + camera.h &&
				((*it)->getHeight() + (*it)->getY())*GameDefinitions::scale > camera.y)
			{
				SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0x0, 0x0, 0xFF);
				SDL_Rect rect;
                SDL_Texture* textureToLoad = nullptr;
                bool renderTexture = false;
                if (dynamic_cast<Trigger*>(*it))
                {
#ifdef DEBUGGAME
                    SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0xFF, 0x0, 0xFF);
#else
                    continue;
#endif
                }
				Creature* tmpC = dynamic_cast<Creature*>(*it);
				if (tmpC != nullptr)
				{
					if (dynamic_cast<PlayerCreature*>(*it))
					{
                        renderTexture = true;
						SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xA5, 0x0, 0xFF);
                        if (tmpC->getSpeedX()<0.0)
                            textureToLoad = sdlWrapper->levelTextureVector[LevelTexture::PlayerLeft];
                        else if (tmpC->getSpeedX()>0.0)
                            textureToLoad = sdlWrapper->levelTextureVector[LevelTexture::PlayerRight];
                        else
                            textureToLoad = sdlWrapper->levelTextureVector[LevelTexture::PlayerFront];
					}
					else
					{
                        renderTexture = true;
                        if (tmpC->getSpeedX()<=0.0)
                            textureToLoad = sdlWrapper->levelTextureVector[LevelTexture::MonsterLeft];
                        else
                            textureToLoad = sdlWrapper->levelTextureVector[LevelTexture::MonsterRight];
						SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xA5, 0x2A, 0x2A, 0xFF);

					}
					if (tmpC->getIsInvulnerable() && SDL_GetTicks() % 2 == 0)
						continue;
					//Disabled due to jittering
					//rect.x = int(((*it)->getX()*game->getPosAlpha() + (*it)->getPrevX()*(1.0 - game->getPosAlpha()))*GameDefinitions::scale) - camera.x;
					//rect.y = int(((*it)->getY()*game->getPosAlpha() + (*it)->getPrevY()*(1.0 - game->getPosAlpha()))*GameDefinitions::scale) - camera.y;
				}
				else
				{
					//rect.x = int((*it)->getX()*GameDefinitions::scale) - camera.x;
					//rect.y = int((*it)->getY()*GameDefinitions::scale) - camera.y;
				}
                if (dynamic_cast<Coin*>(*it))
                    SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0x0, 0xFF);

				rect.x = int((*it)->getX()*GameDefinitions::scale) - camera.x;
				rect.y = int((*it)->getY()*GameDefinitions::scale) - camera.y;
				rect.w = int((*it)->getWidth()*GameDefinitions::scale);
				rect.h = int((*it)->getHeight()*GameDefinitions::scale);
                if (renderTexture)
                    SDL_RenderCopy(sdlWrapper->renderer, textureToLoad, nullptr, &rect);
                else
                    SDL_RenderFillRect(sdlWrapper->renderer, &rect);
			}
		}

        //Draw health meter
        for (int i = game->getPlayerController()->getCreature()->getHealth(); i > 0; --i)
        {
            SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0x0, 0x0, 0xFF);
            SDL_Rect healthRect;
            healthRect.x = i * 30;
            healthRect.y = 20;
            healthRect.h = 20;
            healthRect.w = 20;
            SDL_RenderFillRect(sdlWrapper->renderer, &healthRect);
            SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0x0, 0x0, 0xFF);
            SDL_RenderDrawRect(sdlWrapper->renderer, &healthRect);
        }

        //Draw coin meter
        SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0x0, 0xFF);
        SDL_Rect coinRect = { camera.w - 100, 20, 20, 20 };
        SDL_RenderFillRect(sdlWrapper->renderer, &coinRect);
        SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderDrawRect(sdlWrapper->renderer, &coinRect);
        
        std::string coinString = "x " + std::to_string(dynamic_cast<PlayerCreature*>(game->getPlayerController()->getCreature())->getCoins());
        SDL_Texture* coinText = sdlWrapper->loadTextureFromRenderedText(coinString, { 0x0, 0x0, 0x0, 0xFF }, Font::RegularOutline);
        drawText(coinRect.x + 28, coinRect.y + (coinRect.h / 2) - 2, coinText, TextAlignment::Left);
        SDL_DestroyTexture(coinText);
        coinText = sdlWrapper->loadTextureFromRenderedText(coinString, { 0xFF, 0xFF, 0x0, 0xFF });
        drawText(coinRect.x + 30, coinRect.y + (coinRect.h / 2), coinText, TextAlignment::Left);
        SDL_DestroyTexture(coinText);

	}
	SDL_RenderPresent(sdlWrapper->renderer);
#ifdef DEBUGGAME
    ++countedFrames;
	++otherCountedFrames;
#endif
}

/**
 * Draws menu screen.
 * @return void
 */
void ViewModel::drawMenu()
{
    SDL_ShowCursor(1);
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		SDL_Event quit;
		quit.type = SDL_QUIT;
		SDL_PushEvent(&quit);
	}
	if (state[SDL_SCANCODE_F4])
	{
        //static bool isFull = false;
        //if (!isFull)
        if (!(SDL_GetWindowFlags(sdlWrapper->window) & SDL_WINDOW_FULLSCREEN))
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, SDL_WINDOW_FULLSCREEN);
            //isFull = true;
        }
        else
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, 0);
            //isFull = false;
        }
	}
	SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(sdlWrapper->renderer);
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(sdlWrapper->renderer, &rendererWidth, &rendererHeight);
    drawText(rendererWidth / 2, 100, sdlWrapper->menuTextureVector[MenuTexture::TitleText]);
    drawText(rendererWidth / 2, 160, sdlWrapper->menuTextureVector[MenuTexture::SubtitleText]);
	drawButton(60, rendererHeight - 150, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::StartButton], &Game::startGame);
	drawButton(60, rendererHeight - 100, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::QuitButton], &Game::gameOver);
	SDL_RenderPresent(sdlWrapper->renderer);
#ifdef DEBUGGAME
	++countedFrames;
	++otherCountedFrames;
#endif
}

/**
 * Draws pause screen.
 * @return void
 */
void ViewModel::drawPauseMenu()
{
    SDL_ShowCursor(1);
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_F4])
	{
        if (!(SDL_GetWindowFlags(sdlWrapper->window) & SDL_WINDOW_FULLSCREEN))
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, SDL_WINDOW_FULLSCREEN);
        }
        else
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, 0);
        }
		drawGame();
		hasDrawnPauseMenu = false;
	}
	if (!hasDrawnPauseMenu)
	{
		SDL_RenderCopy(sdlWrapper->renderer, sdlWrapper->menuTextureVector[MenuTexture::PauseMenu], nullptr, nullptr);
	}
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(sdlWrapper->renderer, &rendererWidth, &rendererHeight);
	drawButton(rendererWidth / 2 - 60, rendererHeight - 150, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::ResumeButton], &Game::resumeGame);
	drawButton(rendererWidth / 2 - 60, rendererHeight - 100, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::MenuButton], &Game::quitToMenu);
	
	SDL_RenderPresent(sdlWrapper->renderer);
#ifdef DEBUGGAME
	++countedFrames;
	++otherCountedFrames;
#endif
}

/**
 * Draws failure screen.
 * @return void
 */
void ViewModel::drawLost()
{
    if (!fadeTimer.isStarted())
    {
        fadeTimer.start();
    }
    SDL_ShowCursor(1);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_F4])
    {
        //static bool isFull = false;
        //if (!isFull)
        if (!(SDL_GetWindowFlags(sdlWrapper->window) & SDL_WINDOW_FULLSCREEN))
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, SDL_WINDOW_FULLSCREEN);
            //isFull = true;
        }
        else
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, 0);
            //isFull = false;
        }
        drawGame();
    }
    int fadeAlpha = int(fadeTimer.getTicks()*0.07f);
    int lateFadeAlpha = int(((fadeTimer.getTicks() < 1000) ? 0 : fadeTimer.getTicks() - 1000)*0.03f);
    if (fadeAlpha > SDL_ALPHA_OPAQUE)
        fadeAlpha = SDL_ALPHA_OPAQUE;
    if (lateFadeAlpha > SDL_ALPHA_OPAQUE)
        lateFadeAlpha = SDL_ALPHA_OPAQUE;
    SDL_SetTextureAlphaMod(sdlWrapper->menuTextureVector[MenuTexture::LostMenu], Uint8(fadeAlpha));
    SDL_SetTextureAlphaMod(sdlWrapper->menuTextureVector[MenuTexture::LostText], Uint8(lateFadeAlpha));

    SDL_RenderCopy(sdlWrapper->renderer, sdlWrapper->menuTextureVector[MenuTexture::LostMenu], nullptr, nullptr);

    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(sdlWrapper->renderer, &rendererWidth, &rendererHeight);
    drawText(rendererWidth / 2, 100, sdlWrapper->menuTextureVector[MenuTexture::LostText]);
    drawButton(rendererWidth / 2 - 180, rendererHeight - 100, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::RetryButton], &Game::startGame);
    drawButton(rendererWidth / 2 + 60, rendererHeight - 100, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::MenuButton], &Game::quitToMenu);

    SDL_RenderPresent(sdlWrapper->renderer);
#ifdef DEBUGGAME
    ++countedFrames;
    ++otherCountedFrames;
#endif
}

/**
 * Draws success screen.
 * @return void
 */
void ViewModel::drawWon()
{
    SDL_ShowCursor(1);
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_F4])
    {
        //static bool isFull = false;
        //if (!isFull)
        if (!(SDL_GetWindowFlags(sdlWrapper->window) & SDL_WINDOW_FULLSCREEN))
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, SDL_WINDOW_FULLSCREEN);
            //isFull = true;
        }
        else
        {
            SDL_SetWindowFullscreen(sdlWrapper->window, 0);
            //isFull = false;
        }
    }
    SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdlWrapper->renderer);
    int rendererWidth, rendererHeight;
    SDL_GetRendererOutputSize(sdlWrapper->renderer, &rendererWidth, &rendererHeight);
    drawText(rendererWidth / 2, 100, sdlWrapper->menuTextureVector[MenuTexture::WinText]);
    std::string coinString = "Number of coins you've acquired: " +
        std::to_string(dynamic_cast<PlayerCreature*>(game->getPlayerController()->getCreature())->getCoins()) +
        "/" +
        std::to_string(game->getLevelCoins()) +
        ((dynamic_cast<PlayerCreature*>(game->getPlayerController()->getCreature())->getCoins() == game->getLevelCoins()) ? "!!!" : "");
    SDL_Texture* coinText = sdlWrapper->loadTextureFromRenderedText(coinString, { 0x0, 0x0, 0x0, 0xFF }, Font::Subtitle);
    drawText(50, 180, coinText,TextAlignment::Left);
    SDL_DestroyTexture(coinText);
    drawButton(rendererWidth / 2 - 60, rendererHeight - 100, 120, 25, sdlWrapper->menuTextureVector[MenuTexture::MenuButton], &Game::quitToMenu);
    SDL_RenderPresent(sdlWrapper->renderer);
#ifdef DEBUGGAME
    ++countedFrames;
    ++otherCountedFrames;
#endif
}

/**
 * Draws an interactive button.
 * @param buttonX X position of button
 * @param buttonY Y position of button
 * @param buttonWidth width of button
 * @param buttonHeight height of button
 * @param texture a pointer to texture that should be used on button
 * @param onClick a member function pointer to Game member function that should be called upon button press
 * @return void
 */
void ViewModel::drawButton(int buttonX, int buttonY, int buttonWidth, int buttonHeight, SDL_Texture* texture, void (Game::*onClick)())
{
	int mouseX, mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	SDL_Rect button = { buttonX, buttonY, buttonWidth, buttonHeight };

	//Check if mouse cursor is over button
	if (mouseX > button.x &&
		mouseX < button.x + button.w &&
		mouseY > button.y &&
		mouseY < button.y + button.h)
	{
		SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0xBF, 0xFF, 0xFF);
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xA5, 0x0, 0xFF);
			(game->*onClick)();
		}
	}
	else
		SDL_SetRenderDrawColor(sdlWrapper->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(sdlWrapper->renderer, &button);
	SDL_SetRenderDrawColor(sdlWrapper->renderer, 0x0, 0x0, 0x0, 0xFF);
    drawText(button.x + (button.w / 2), (button.y + button.h / 2), texture);
	SDL_RenderDrawRect(sdlWrapper->renderer, &button);
}

/**
 * Draws text.
 * @param x X position of text
 * @param y Y position of text
 * @param texture a pointer to texture that should be drawn (preferably a texture representing text)
 * @param align how text should be aligned meaning how x and y values should be treated.
 * @return void
 */
void ViewModel::drawText(int x, int y, SDL_Texture* texture, TextAlignment align)
{
    SDL_Rect textTextureRect;
    SDL_QueryTexture(texture, nullptr, nullptr, &textTextureRect.w, &textTextureRect.h);
    switch (align)
    {
    case TextAlignment::Center:
        textTextureRect.x = x - (textTextureRect.w / 2);
        break;
    case TextAlignment::Left:
        textTextureRect.x = x;
        break;
    case TextAlignment::Right:
        textTextureRect.x = x - textTextureRect.w;
        break;
    }
    textTextureRect.y = y - (textTextureRect.h / 2);
    SDL_RenderCopy(sdlWrapper->renderer, texture, nullptr, &textTextureRect);
}

/**
 * Calculate an average number of frames per second.
 * @return void
 */
void ViewModel::correctFPS()
{
    avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000)
    {
        avgFPS = 0;
    }
}

/**
 * Calculate a number of frames per second.
 * @return void
 */
void ViewModel::currentFPS()
{
    if (curFPSTimer.getTicks() / 1000.f > 1.0f)
    {
        curFPS = otherCountedFrames;
        otherCountedFrames = 0;
        curFPSTimer.start();
    }
}
