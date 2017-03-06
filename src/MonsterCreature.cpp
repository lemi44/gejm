#include "MonsterCreature.h"
#include "PlayerCreature.h"

/**
 * MonsterCreature implementation
 */

/**
 * The default constructor of MonsterCreature.
 * @param x X position of MonsterCreature. Defaults to 0.0
 * @param y Y position of MonsterCreature. Defaults to 0.0
 * @param width width of MonsterCreature. Defaults to 1.0
 * @param height height of MonsterCreature. Defaults to 1.0
 * @param health health of MonsterCreature. Defaults to 1
 * @see Creature
 */
MonsterCreature::MonsterCreature(double x, double y, double width, double height, Uint8 health) :
    Creature(x, y, width, height, health)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MonsterCreature created!");
}

/**
 * Default destructor
 */
MonsterCreature::~MonsterCreature()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MonsterCreature destroyed!");
}

/**
 * Function for resolving special cases of collision.
 * If special case is not found, function calls Creature implementation of onCollision.
 * @param collider a pointer to a SolidObject with which MonsterCreature is colliding
 * @return void
 */
void MonsterCreature::onCollision(SolidObject* collider)
{
	PlayerCreature* tmpPlayerCreature = dynamic_cast<PlayerCreature*>(collider);
	if (tmpPlayerCreature != nullptr)
	{
		//some debug messages
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"I'm a generic monster!");
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "HurtCollision: ME: X:%f Y:%f HIM: X:%f Y:%f\n", x, y, tmpPlayerCreature->getX(), tmpPlayerCreature->getY());

		tmpPlayerCreature->hurt(1);
		return;
	}
	Creature::onCollision(collider);
}