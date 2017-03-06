#ifndef MONSTERCREATURE_H
#define MONSTERCREATURE_H

#include "Creature.h"

/**
 * MonsterCreature is a Creature that should be AI controlled as it hurts PlayerCreature.
 * It derives from Creature.
 * @see Creature
 */
class MonsterCreature :
	public Creature
{
public:
    /**
     * The default constructor of MonsterCreature.
     * @param x X position of MonsterCreature. Defaults to 0.0
     * @param y Y position of MonsterCreature. Defaults to 0.0
     * @param width width of MonsterCreature. Defaults to 1.0
     * @param height height of MonsterCreature. Defaults to 1.0
     * @param health health of MonsterCreature. Defaults to 1
     * @see Creature
     */
	MonsterCreature(double x = 0.0, double y = 0.0, double width = 1.0, double height = 1.0, Uint8 health = 1);

    /**
     * Default destructor
     */
	~MonsterCreature();

    /**
     * Function for resolving special cases of collision.
     * If special case is not found, function calls Creature implementation of onCollision.
     * @param collider a pointer to a SolidObject with which MonsterCreature is colliding
     * @return void
     */
    void onCollision(SolidObject* collider) override;

    /**
     * Assignment operator is deleted because of constant member initialized on Construction.
     */
    MonsterCreature& operator=(MonsterCreature const&) = delete;
};

#endif // MONSTER_H