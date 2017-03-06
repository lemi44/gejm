#ifndef CREATURE_H
#define CREATURE_H

#include "SolidObject.h"
#include "Timer.h"

/**
 * The types of states that can represent a collision of a Creature.
 * Multiple states can be combined by treating them as bit fields.
 * @see Creature
 */
enum class CollisionState
{
    /**
     * There is no collision.
     */
    None = 0x0,

    /**
     * Creature is colliding with something that is below Creature.
     */
    FromAbove = 0x1,

    /**
     * Creature is colliding with something that is above Creature.
     */
    FromBelow = 0x2,

    /**
     * Creature is colliding with something that is to the right of Creature.
     */
    FromLeft = 0x4,

    /**
     * Creature is colliding with something that is to the left of Creature.
     */
    FromRight = 0x8
};

/**
 * OR operator overload so CollisionState can be used as a flag field.
 * @see CollisionState
 */
inline CollisionState operator| (CollisionState a, CollisionState b)
{
    // Cast to int first otherwise we'll just end up recursing
    return static_cast<CollisionState>(static_cast<int>(a) | static_cast<int>(b));
}

/**
 * AND operator overload so CollisionState can be used as a flag field.
 * @see CollisionState
 */
inline CollisionState operator& (CollisionState a, CollisionState b)
{
    return static_cast<CollisionState>(static_cast<int>(a)& static_cast<int>(b));
}

/**
 * OR assignment operator overload so CollisionState can be used as a flag field.
 * @see CollisionState
 */
inline CollisionState& operator|= (CollisionState& a, CollisionState b)
{
    return a = a | b;
}

/**
 * Creature is the base class of all creatures that can be possessed by players or AI.
 * They are the physical representations of players and creatures in a level.
 * It derives from SolidObject.
 * @see SolidObject
 */
class Creature :
    public SolidObject
{
public:
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
    Creature(double x = 0.0, double y = 0.0, double width = 1.0, double height = 1.0, Uint8 health = 1);
    
    /**
     * The default destructor.
     */
    virtual ~Creature();

    /**
     * Changes members that hold information about previous step to hold current state of Creature.
     * Also checks if invTimer has passed and disables invulnerability.
     * Sets current collisionState to noCollision.
     * @return void
     */
    void savePrevious() override;

    /**
     * Changes relatively current position of Creature.
     * @param x relative change of position on X axis
     * @param y relative change of position on Y axis
     * @return void
     */
    void moveBy(double x, double y);
    
    /**
     * Changes health of Creature.
     * If damage equals 127 or is less than or equal to 0 then it ignores invulnerability.
     * Else deal damage only if Creature is not Invulnerable and set invulnerability of Creature.
     * @param damage how much damage Creature takes
     * @return void
     */
    void hurt(Sint8 damage);

    /**
     * Changes current velocity vector of Creature.
     * @param x new velocity value on X axis
     * @param y new velocity value on Y axis
     * @return void
     */
    void setSpeedVector(double x, double y);
    
    /**
     * Changes current collisionState of Creature by OR assignment of parameter.
     * @param state CollisionState to be added to member collisionState
     * @return void
     */
    void addCollisionState(CollisionState state);

    /**
     * Virtual function for resolving collision.
     * Default implementation ignores Coin and Trigger and solves collision with SolidObject. 
     * @param collider a pointer to a SolidObject with which Creature is colliding
     * @return void
     */
    virtual void onCollision(SolidObject* collider);

    /**
     * Get current health of Creature.
     * @return Uint8
     */
    Uint8 getHealth() const;

    /**
     * Check if Creature is alive.
     * @return bool
     */
    bool getIsAlive() const;
    
    /**
     * Check if Creature is invulnerable.
     * @return bool
     */
    bool getIsInvulnerable() const;

    /**
     * Check if Creature was invulnerable.
     * @return bool
     */
    bool getWasInvulnerable() const;

    /**
     * Get current velocity of Creature on X-axis.
     * @return double
     */
    double getSpeedX() const;

    /**
     * Get current velocity of Creature on Y-axis.
     * @return double
     */
    double getSpeedY() const;
    
    /**
     * Get current collisionState of Creature.
     * @return CollisionState
     */
    CollisionState getCollisionState() const;

    /**
     * Assignment operator is deleted because of constant member initialized on Construction.
     */
    Creature& operator=(Creature const&) = delete;

protected:
    /**
     * Current health of Creature.
     */
    Uint8 health;
    
    /**
     * Constant number of invulnerability frames of Creature.
     */
    Uint32 const invFrames;

    /**
     * Current liveness of Creature.
     */
    bool isAlive;

    /**
     * Current state of invulnerability of Creature.
     */
    bool isInvulnerable;
    
    /**
     * Previous state of invulnerability of Creature.
     */
    bool wasInvulnerable;
    
    /**
     * Current velocity on X-axis of Creature.
     */
    double speedX;

    /**
     * Current velocity on Y-axis of Creature.
     */
    double speedY;

    /**
     * Current CollisionState of Creature.
     */
    CollisionState collisionState;

    /**
     * Timer used to calculate a period of invulnerability of Creature.
     */
    Timer invTimer;
};

#endif // CREATURE_H
