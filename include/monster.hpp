#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include <npc.hpp>
#include <raylib.h>

class Monster: public NPC{
    public:
    Monster();

    bool update() override;
    void setGroup(string group) override;
    virtual void applyCollisionRoles(Object *object, Vector2 normal);

    Vector2 directionMovement;

    static void directionCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void playerCollisionCallback(Object *self, Object *object, Vector2 normal);
};

#endif