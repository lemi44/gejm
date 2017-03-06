#include "Creature.h"
#include "Coin.h"
#include "Trigger.h"

/**
 * Creature implementation
 */

/**
 * The default constructor of Creature.
 * @param x X position of Creature. Defaults to 0.0
 * @param y Y position of Creature. Defaults to 0.0
 * @param width width of Creature. Defaults to 1.0
 * @param height height of Creature. Defaults to 1.0
 * @param health health of Creature. Defaults to 1
 * @see SolidObject
 * @see health
 */
Creature::Creature(double x, double y, double width, double height, Uint8 health) :
	SolidObject(x, y, width, height),
	health(health),
	invFrames(1500),
	isAlive(true),
	isInvulnerable(false),
	wasInvulnerable(false),
	speedX(0.0),
    speedY(0.0),
	collisionState(CollisionState::None)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creature created at x:%f, y:%f, with witdh:%f, height:%f and health:%d", x, y, width, height, health);
}

/**
 * The default destructor.
 */
Creature::~Creature()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creature destroyed!");
}

/**
 * Changes members that hold information about previous step to hold current state of Creature.
 * Also checks if invTimer has passed and disables invulnerability.
 * Sets current collisionState to noCollision.
 * @return void
 */
void Creature::savePrevious()
{
    collisionState = CollisionState::None;
    wasInvulnerable = isInvulnerable;
    if (isInvulnerable)
        if (invTimer.getTicks() > invFrames)
        {
            isInvulnerable = false;
            invTimer.stop();
        }
    Object::savePrevious();
}

/**
 * Changes relatively current position of Creature.
 * @param x relative change of position on X axis
 * @param y relative change of position on Y axis
 * @return void
 */
void Creature::moveBy(double x, double y)
{
    this->x += x;
    this->y += y;
}

/**
 * Changes health of Creature.
 * If damage equals 127 or is less than or equal to 0 then it ignores invulnerability.
 * Else deal damage only if Creature is not Invulnerable and set invulnerability of Creature.
 * @param damage how much damage Creature takes
 * @return void
 */
void Creature::hurt(Sint8 damage)
{ 
	if (!isInvulnerable || damage==127)
	{
        health -= (health - damage < 0) ? health : damage;
		if (health <= 0)
			isAlive = false;
		isInvulnerable = true;
		invTimer.start();
	}
	
}

/**
 * Changes current velocity vector of Creature.
 * @param x new velocity value on X axis
 * @param y new velocity value on Y axis
 * @return void
 */
void Creature::setSpeedVector(double x, double y)
{
    speedX=x;
    speedY=y;
}

/**
 * Changes current collisionState of Creature by OR assignment of parameter.
 * @param state CollisionState to be added to member collisionState
 * @return void
 */
void Creature::addCollisionState(CollisionState state)
{
    collisionState |= state;
}

/**
 * Virtual function for resolving collision.
 * Default implementation ignores Coin and Trigger and solves collision with SolidObject.
 * @param collider a pointer to a SolidObject with which Creature is colliding
 * @return void
 */
void Creature::onCollision(SolidObject* collider)
{
    //collider is guaranteed here to not be a nullptr but just to be safe
    if (collider == nullptr)
        return;

	//here ignore coin
    if (dynamic_cast<Coin*>(collider))
        return;

    //here ignore trigger
    if (dynamic_cast<Trigger*>(collider))
        return;

	double tmpX = speedX;
	double tmpY = speedY;

	double enterY = (y + height) - collider->getY();
	double enterX = (x + width) - collider->getX();
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "My X:%f My Y:%f SpeedX:%f SpeedY:%f EnterX:%f EnterY:%f\n", x, y, tmpX, tmpY, enterX, enterY);
	if (enterY > 0 && (y + height - 0.2 < collider->getY() || y > collider->getY() + collider->getHeight() - 0.2))
	{
		if ((y + height / 2.0) <= (collider->getY() + collider->getHeight() / 2.0))
		{
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "His Height:%f\n", collider->getY() + collider->getHeight() / 2);
			moveBy(0, -enterY);
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Move up");
			collisionState |= CollisionState::FromAbove;
		}
		else
		{
			moveBy(0, (collider->getY() + collider->getHeight()) - y);
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Move down");
			collisionState |= CollisionState::FromBelow;
		}
		setSpeedVector(tmpX, 0.0);
		return;
	}
	tmpY = speedY;
	if (enterX > 0)
	{
		if ((x + width / 2.0) <= (collider->getX() + collider->getWidth() / 2.0))
		{
			moveBy(-enterX, 0);
			collisionState |= CollisionState::FromLeft;
		}
		else
		{
			moveBy((collider->getX() + collider->getWidth()) - x, 0);
			collisionState |= CollisionState::FromRight;
		}
		setSpeedVector(0.0, tmpY);
	}
}

/**
 * Get current health of Creature.
 * @return Uint8
 */
Uint8 Creature::getHealth() const
{
    return health;
}

/**
 * Check if Creature is alive.
 * @return bool
 */
bool Creature::getIsAlive() const
{
    return isAlive;
}

/**
 * Check if Creature is invulnerable.
 * @return bool
 */
bool Creature::getIsInvulnerable() const
{
    return isInvulnerable;
}

/**
 * Check if Creature was invulnerable.
 * @return bool
 */
bool Creature::getWasInvulnerable() const
{
    return wasInvulnerable;
}

/**
 * Get current velocity of Creature on X-axis.
 * @return double
 */
double Creature::getSpeedX() const
{
    return speedX;
}

/**
 * Get current velocity of Creature on Y-axis.
 * @return double
 */
double Creature::getSpeedY() const
{
    return speedY;
}

/**
 * Get current collisionState of Creature.
 * @return CollisionState
 */
CollisionState Creature::getCollisionState() const
{
    return collisionState;
}
