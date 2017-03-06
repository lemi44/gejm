#include "PlayerController.h"
#include "GameDefs.h"

/**
 * PlayerController implementation
 */

/**
 * The default constructor with PlayerCreature possesion.
 * @param creature association of Creature object
 * @param maxSpeed an absolute value of a maximum horizontal velocity that PlayerController should set
 * @see PlayerCreature
 * @see Controller
 */
PlayerController::PlayerController(PlayerCreature* creature) :
    Controller(creature, 5.0),
    doJump(false),
    stopJump(false),
    grounded(false),
    jumpShortSpeed(3.0),
    jumpSpeed(7.0)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PlayerController created!");
}

/**
 * The default destructor.
 */
PlayerController::~PlayerController()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "PlayerController destroyed!");
}

/**
 * Tell controlled PlayerCreature to jump.
 * @return void
 */
void PlayerController::jump()
{
	/*if (!hasJumped)
	{
		if (jumpTimer.getTicks() == 0)
			jumpTimer.start();
		else if (jumpTimer.getTicks() > 400)
		{
			hasJumped = true;
		}
		creature->setSpeedVector(creature->getSpeedX(), -4.0);
	}*/

	//Normal jump (full speed)
	if (doJump)
	{
		creature->setSpeedVector(creature->getSpeedX(), -jumpSpeed);
		doJump = false;
		grounded = false;
	}

	//Cancel the jump when the button is no longer pressed
	if (stopJump)
	{
		if (creature->getSpeedY() < -jumpShortSpeed)
			creature->setSpeedVector(creature->getSpeedX(), -jumpShortSpeed);
		stopJump = false;
	}
}

/**
 * An implementation of how to control associated PlayerCreature.
 * @see PlayerCreature
 * @see Controller
 * @see jump()
 * @return void
 */
void PlayerController::control()
{
	if ((creature->getCollisionState() & CollisionState::FromAbove) == CollisionState::FromAbove)
	{
		//hasJumped = false;
		//jumpTimer.stop();
		grounded = true;
	}
	//else
	//{
	//	grounded = false;
	//}
    //else if ((creature->getCollisionState() & CollisionState::fromBelowCollision) == CollisionState::fromBelowCollision)
    //{
        //hasJumped = true;
    //}
    inputHandling();
	jump();
	//creature->move();
}

/**
* Interpret player input.
* @return void
*/
void PlayerController::inputHandling()
{
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_LEFT])
		goLeft();
	else if (state[SDL_SCANCODE_RIGHT])
		goRight();
	else
		stopGoing();
	if (state[SDL_SCANCODE_SPACE])
	{
		if (grounded)
			doJump = true;
	}
	else
	{
		if (!grounded)
			stopJump = true;
		/*if (jumpTimer.getTicks() > 0)
			hasJumped = true;*/
	}
}