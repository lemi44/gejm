#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Controller.h"
#include "PlayerCreature.h"
#include "Timer.h"
#include <SDL_keyboard.h>
#include <SDL_events.h>

/**
 * PlayerController is a special Controller that takes interprets player input.
 */
class PlayerController: public Controller
{
public: 
    
    /**
     * The default constructor with PlayerCreature possesion.
     * @param creature association of Creature object
     * @param maxSpeed an absolute value of a maximum horizontal velocity that PlayerController should set
     * @see PlayerCreature
     * @see Controller
     */
    PlayerController(PlayerCreature* creature);
    
    /**
     * The default destructor.
     */
    ~PlayerController();
    
    /**
     * Tell controlled PlayerCreature to jump.
     * @return void
     */
    void jump();
    
    /**
     * An implementation of how to control associated PlayerCreature.
     * @see PlayerCreature
     * @see Controller
     * @see jump()
     * @return void
     */
    void control() override;

    /**
     * Assignment operator is deleted because PlayerController has constant variable.
     */
    PlayerController& operator=(PlayerController const&) = delete;

private:
    /**
     * Should controlled PlayerCreature jump.
     */
	bool doJump;
	
    /**
     * Should controlled PlayerCreature cancel jump.
     */
    bool stopJump;

    /**
     * Is controlled PlayerCreature touching ground.
     */
	bool grounded;

    /**
     * Vertical velocity for shortest jump.
     */
	double jumpShortSpeed;

    /**
     * Vertical velocity for longest jump.
     */
	double jumpSpeed;

    /**
     * Interpret player input.
     * @return void
     */
	void inputHandling();
};

#endif // PLAYERCONTROLLER_H
