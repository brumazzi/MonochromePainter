#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <npc.hpp>

class Player: public NPC{
    public:
    Player();

    bool update() override;
    void applyCollisionRoles(Object *object, Vector2 normal) override;

    static void itemCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void respawnCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void jumpCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void monsterCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void nextLevelCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void damageCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void windCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void platformCollisionCallback(Object *self, Object *object, Vector2 normal);

    private:
};

#endif