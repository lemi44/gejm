#include "Controller.h"

/**
 * Controller implementation
 */

/**
 * The default constructor with Creature possesion.
 * @param creature association of Creature object.
 * @param maxSpeed an absolute value of a maximum horizontal velocity that Controller should set.
 * @see Creature
 * @see maxSpeed
 */
Controller::Controller(Creature* creature, double maxSpeed) :
    creature(creature),
    controllerState(ControllerState::GoingLeft),
    maxSpeed(maxSpeed)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Controller created with maxSpeed:%f", maxSpeed);
}

/**
 * The default destructor.
 */
Controller::~Controller()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Controller destroyed!");
}

/**
 * A virtual implementation of how to control associated Creature.
 * @see Creature
 * @see goLeft()
 * @see goRight()
 * @see stopGoing()
 * @return void
 */
void Controller::control()
{
    if ((creature->getCollisionState() & CollisionState::FromLeft) == CollisionState::FromLeft ||
        (creature->getCollisionState() & CollisionState::FromRight) == CollisionState::FromRight)
    {
        if (controllerState == ControllerState::GoingLeft)
            controllerState = ControllerState::GoingRight;
        else
            controllerState = ControllerState::GoingLeft;
    }
    switch (controllerState)
    {
    case ControllerState::GoingLeft:
        goLeft();
        break;
    case ControllerState::GoingRight:
        goRight();
        break;
    case ControllerState::NotGoing:
    default:
        stopGoing();
    }
    //creature->move();
}

/**
 * Get associated Creature.
 * @see Creature
 * @return Creature* a pointer to associated Creature
 */
Creature* Controller::getCreature()
{
    return creature;
}

/**
* Get current state of controller
* @return ControllerState
*/
ControllerState Controller::getControllerState()
{
    return controllerState;
}

/**
 * Make associated Creature go left.
 * @see Creature
 * @return void
 */
void Controller::goLeft()
{
    controllerState = ControllerState::GoingLeft;
    creature->setSpeedVector(-1.0*maxSpeed, creature->getSpeedY());
}

/**
 * Make associated Creature go right.
 * @see Creature
 * @return void
 */
void Controller::goRight()
{
    controllerState = ControllerState::GoingRight;
    creature->setSpeedVector(1.0*maxSpeed, creature->getSpeedY());
}

/**
 * Make associated Creature stop going.
 * @see Creature
 * @return void
 */
void Controller::stopGoing()
{
    controllerState = ControllerState::NotGoing;
    creature->setSpeedVector(0.0, creature->getSpeedY());
}
