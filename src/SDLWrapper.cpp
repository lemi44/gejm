#include "SDLWrapper.h"
#include <algorithm>
#include <ctime>
#include <cstring>

/**
 * SDLWrapper implementation
 */

/**
 * Default constructor that initializes SDL, it's plugins and also loads and stores fonts and textures.
 */
SDLWrapper::SDLWrapper():
	window(nullptr),
	renderer(nullptr)
{
    srand(time(nullptr));

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw InitError();
	}
	else
	{
#ifdef DEBUGGAME
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif
        //Create window
		window = SDL_CreateWindow(GameDefinitions::gameTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GameDefinitions::screenWidth, GameDefinitions::screenHeight, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			throw InitError();
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr)
			{
				throw InitError();
			}
			else
			{
                //Set render quality
                SDL_Log("Setting best render quality...");
                if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best"))
                {
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set best render quality, reverting to linear...");
                    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"))
                    {
                        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't set linear render quality, reverting to nearest...");
                    }
                }
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					throw InitError(TTF_GetError());
				}
				else
				{
					loadFonts();
                    loadMenuTextures();

                    if (!(IMG_Init(IMG_InitFlags::IMG_INIT_PNG)&IMG_InitFlags::IMG_INIT_PNG))
                    {
                        throw InitError(IMG_GetError());
                    }
                    else
                    {
                        //Here it is safe to load textures from files
                        loadLevelTextures();
                    }
				}

			}
		}
	}
}

/**
 * Default destructor that unloads every loaded resource and quits initialized SDL subsystems.
 */
SDLWrapper::~SDLWrapper()
{
    unloadFonts();
    unloadMenuTextures();
    unloadLevelTextures();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

/**
 * Loads texture from rendered text.
 * @param textureText string with text to render
 * @param textColor color of text
 * @param font which font to use. Defaults to Font::Regular
 * @return SDL_Texture* a pointer to loaded texture
 * @see Font
 */
SDL_Texture* SDLWrapper::loadTextureFromRenderedText(std::string textureText, SDL_Color textColor, Font font)
{
    SDL_Texture* texture = nullptr;
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(fontVector[font], textureText.c_str(), textColor);
    if (textSurface == nullptr)
    {
        throw InitError(TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr)
        {
            throw InitError();
        }
        else
        {
            //Get image dimensions
            //width = textSurface->w;
            //height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return texture;
}

/**
 * Loads texture from file.
 * @param path string with path to file
 * @return SDL_Texture* a pointer to loaded texture
 */
SDL_Texture* SDLWrapper::loadTextureFromFile(std::string path)
{
    SDL_Texture* texture = nullptr;
    //Render image surface
    SDL_Surface* imageSurface = IMG_Load(path.c_str());
    if (imageSurface == nullptr)
    {
        throw InitError(IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        if (texture == nullptr)
        {
            throw InitError();
        }
        else
        {
            //Get image dimensions
            //width = textSurface->w;
            //height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(imageSurface);
    }

    //Return success
    return texture;
}

/**
 * Loads all level textures.
 * @return void
 */
void SDLWrapper::loadLevelTextures()
{
    if (!levelTextureVector.empty())
    {
        unloadLevelTextures();
    }

    //Loading PlayerLeft
    levelTextureVector.push_back(loadTextureFromFile("Data/images/PLAYER_L.png"));
    if (levelTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading PlayerRight
    levelTextureVector.push_back(loadTextureFromFile("Data/images/PLAYER_R.png"));
    if (levelTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading PlayerFront
    levelTextureVector.push_back(loadTextureFromFile("Data/images/PLAYER_F.png"));
    if (levelTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading MonsterLeft
    levelTextureVector.push_back(loadTextureFromFile("Data/images/MONSTER_L.png"));
    if (levelTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading MonsterRight
    levelTextureVector.push_back(loadTextureFromFile("Data/images/MONSTER_R.png"));
    if (levelTextureVector.back() == nullptr)
    {
        throw InitError();
    }
}

/**
 * Unloads all level textures.
 * @return void
 */
void SDLWrapper::unloadLevelTextures()
{
    std::for_each(levelTextureVector.begin(), levelTextureVector.end(), SDL_DestroyTexture);
    levelTextureVector.clear();
}

/**
 * Loads all fonts.
 * @return void
 */
void SDLWrapper::loadFonts()
{
	if (!fontVector.empty())
	{
		unloadFonts();
	}
    //Loading regular
    fontVector.push_back(TTF_OpenFont("Data/fonts/OpenSans-Regular.ttf", 16));
    if (fontVector.back() == nullptr)
    {
        throw InitError();
    }
    TTF_SetFontHinting(fontVector[Font::Regular], TTF_HINTING_LIGHT);
    //Loading title
    fontVector.push_back(TTF_OpenFont("Data/fonts/OpenSans-Regular.ttf", 72));
    if (fontVector.back() == nullptr)
    {
        throw InitError();
    }
    TTF_SetFontHinting(fontVector[Font::Title], TTF_HINTING_LIGHT);
    //Loading subtitle
    fontVector.push_back(TTF_OpenFont("Data/fonts/OpenSans-Regular.ttf", 24));
    if (fontVector.back() == nullptr)
    {
        throw InitError();
    }
    TTF_SetFontHinting(fontVector[Font::Subtitle], TTF_HINTING_LIGHT);
    //Loading regularOutline
    fontVector.push_back(TTF_OpenFont("Data/fonts/OpenSans-Regular.ttf", 16));
    if (fontVector.back() == nullptr)
    {
        throw InitError();
    }
    TTF_SetFontHinting(fontVector[Font::RegularOutline], TTF_HINTING_LIGHT);
    TTF_SetFontOutline(fontVector[Font::RegularOutline], 2);
}

/**
 * Unloads all fonts.
 * @return void
 */
void SDLWrapper::unloadFonts()
{
	std::for_each(fontVector.begin(), fontVector.end(), TTF_CloseFont);
	fontVector.clear();
}

/**
 * Loads all menu textures.
 * @return void
 */
void SDLWrapper::loadMenuTextures()
{
    if (!menuTextureVector.empty())
    {
        unloadMenuTextures();
    }

    //Loading PauseMenu

    menuTextureVector.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    else
    {
        // Initialize texture pixels to a grey transparent RGBA value
        unsigned char* bytes = nullptr;
        int pitch = 0;
        SDL_LockTexture(menuTextureVector[MenuTexture::PauseMenu], nullptr, reinterpret_cast<void**>(&bytes), &pitch);

        Uint8 abgr[4] = { 0xAA, 0x0, 0x0, 0x0 }; //inverted RGBA due to using little-endian machine as target
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                memcpy(&bytes[(y * 8 + x)*sizeof(abgr)], abgr, sizeof(abgr));
            }
        }
        SDL_UnlockTexture(menuTextureVector[MenuTexture::PauseMenu]);
        SDL_SetTextureBlendMode(menuTextureVector[MenuTexture::PauseMenu], SDL_BLENDMODE_BLEND);
    }

    //Loading LostMenu

    menuTextureVector.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    else
    {
        // Initialize texture pixels to a black opaque RGBA value
        unsigned char* bytes = nullptr;
        int pitch = 0;
        SDL_LockTexture(menuTextureVector[MenuTexture::LostMenu], nullptr, reinterpret_cast<void**>(&bytes), &pitch);

        Uint8 abgr[4] = { 0xFF, 0x0, 0x0, 0x0 }; //inverted RGBA due to using little-endian machine as target
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                memcpy(&bytes[(y * 8 + x)*sizeof(abgr)], abgr, sizeof(abgr));
            }
        }
        SDL_UnlockTexture(menuTextureVector[MenuTexture::LostMenu]);
        SDL_SetTextureBlendMode(menuTextureVector[MenuTexture::LostMenu], SDL_BLENDMODE_BLEND);
    }

    //Loading font-based textures

    //Loading startButtonTexture
    menuTextureVector.push_back(loadTextureFromRenderedText("Start", { 0x0, 0x0, 0x0, 0xFF }));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading resumeButtonTexture
    menuTextureVector.push_back(loadTextureFromRenderedText("Resume", { 0x0, 0x0, 0x0, 0xFF }));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading menuButtonTexture
    menuTextureVector.push_back(loadTextureFromRenderedText("Quit to Menu", { 0x0, 0x0, 0x0, 0xFF }));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading quitButtonTexture
    menuTextureVector.push_back(loadTextureFromRenderedText("Quit Game", { 0x0, 0x0, 0x0, 0xFF }));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading retryButtonTexture
    menuTextureVector.push_back(loadTextureFromRenderedText("Retry", { 0x0, 0x0, 0x0, 0xFF }));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading titleTextTexture
    menuTextureVector.push_back(loadTextureFromRenderedText(GameDefinitions::gameTitle, { 0x0, 0x0, 0x0, 0xFF }, Font::Title));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading subtitleTextTexture
    menuTextureVector.push_back(loadTextureFromRenderedText(GameDefinitions::gameSubtitles[(rand() % GameDefinitions::gameSubtitlesSize)], { 0x0, 0x0, 0x0, 0xFF }, Font::Subtitle));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading lostTextTexture
    menuTextureVector.push_back(loadTextureFromRenderedText(GameDefinitions::gameLost, { 0xFF, 0x0, 0x0, 0xFF }, Font::Title));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
    //Loading winTextTexture
    menuTextureVector.push_back(loadTextureFromRenderedText(GameDefinitions::gameWon, { 0x0, 0x0, 0x0, 0xFF }, Font::Title));
    if (menuTextureVector.back() == nullptr)
    {
        throw InitError();
    }
}

/**
 * Unloads all menu textures.
 * @return void
 */
void SDLWrapper::unloadMenuTextures()
{
    std::for_each(menuTextureVector.begin(), menuTextureVector.end(), SDL_DestroyTexture);
    menuTextureVector.clear();
}
