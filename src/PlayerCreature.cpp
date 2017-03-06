#include "PlayerCreature.h"
#include "Coin.h"
#include "Trigger.h"

/**
 * PlayerCreature implementation
 */

/**
 * The default constructor of PlayerCreature.
 * @param x X position of PlayerCreature. Defaults to 0.0
 * @param y Y position of PlayerCreature. Defaults to 0.0
 * @param width width of PlayerCreature. Defaults to 1.0
 * @param height height of PlayerCreature. Defaults to 1.0
 * @param health health of PlayerCreature. Defaults to 3
 * @see Creature
 */
PlayerCreature::PlayerCreature(double x, double y, double width, double height, Uint8 health) :
    Creature(x, y, width, height, health),
    coins(0)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PlayerCreature created!");
}

/**
 * Default destructor
 */
PlayerCreature::~PlayerCreature()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PlayerCreature destroyed!");
}

/**
 * Function for resolving special cases of collision.
 * If special case is not found, function calls Creature implementation of onCollision.
 * @param collider a pointer to a SolidObject with which MonsterCreature is colliding
 * @return void
 */
void PlayerCreature::onCollision(SolidObject* collider)
{
    Trigger* tmpT = dynamic_cast<Trigger*>(collider);
    if (tmpT != nullptr)
    {
        if (!tmpT->getDestroyed())
        {
            tmpT->trigger();
        }
    }

	//here do something if got a coin
	Coin* tmpCoin = dynamic_cast<Coin*>(collider);
	if (tmpCoin != nullptr)
	{
		if (!tmpCoin->getDestroyed())
		{
			++coins;
			tmpCoin->destroy();
		}
		return;
	}

	Creature* tmpCreature = dynamic_cast<Creature*>(collider);
	if (tmpCreature != nullptr)
	{
		//if collided with creature, ignore it
		return;
	}

	Creature::onCollision(collider);
}

/**
 * Get number of collected coins.
 * @return Uint32
 */
Uint32 PlayerCreature::getCoins() const
{
	return coins;
}