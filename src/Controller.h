#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Creature.h"

/**
 * The types of states that can represent a controlled Creature.
 * @see Controller
 * @see Creature
 */
enum class ControllerState
{
    /**
     * Controlled creature is going left.
     */
    GoingLeft,

    /**
     * Controlled creature is going right.
     */
    GoingRight,

    /**
     * Controlled creature isn't going anywhere.
     */
    NotGoing
};

/**
 * Controller is a base class that can control a Creature.
 * Default implementation move in one direction then change it after collision.
 */
class Controller
{
public:

    /**
     * The default constructor with Creature possesion.
     * @param creature association of Creature object
     * @param maxSpeed an absolute value of a maximum horizontal velocity that Controller should set
     * @see Creature
     * @see maxSpeed
     */
    Controller(Creature* creature, double maxSpeed = 1.0);

    /**
     * The default destructor.
     */
    virtual ~Controller();

    /**
     * A virtual implementation of how to control associated Creature.
     * @see Creature
     * @see goLeft()
     * @see goRight()
     * @see stopGoing()
     * @return void
     */
    virtual void control();

    /**
     * Get associated Creature.
     * @see Creature
     * @return Creature* a pointer to associated Creature
     */
    Creature* getCreature();

    /**
     * Get current state of controller
     * @return ControllerState
     */
    ControllerState getControllerState();
protected:
    /**
     * a protected pointer to associated Creature
     */
    Creature* creature;

    /**
     * a protected ControllerState variable
     * @see ControllerState
     */
    ControllerState controllerState;

    /**
     * an absolute value of a maximum horizontal velocity that Controller should set
     */
    double const maxSpeed;

    /**
     * Make associated Creature go left.
     * @see Creature
     * @return void
     */
    void goLeft();

    /**
     * Make associated Creature go right.
     * @see Creature
     * @return void
     */
    void goRight();

    /**
     * Make associated Creature stop going.
     * @see Creature
     * @return void
     */
    void stopGoing();
    
    /**
     * Assignment operator is deleted because Controller has constant variable.
     */
    Controller& operator=(Controller const&) = delete;
};

#endif // CONTROLLER_H
