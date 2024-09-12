#ifndef __DYNAMIC_GROUND_HPP__
#define __DYNAMIC_GROUND_HPP__

#include <object.hpp>

class DynamicGround: public Object{
    public:
    DynamicGround();

    bool update() override;
    void setGroup(string group) override;
    Vector2 getDirectionMovement();


    void applyCollisionRoles(Object *object, Vector2 normal) override;

    static void invertDirectionCollisionCallback(Object *self, Object *object, Vector2 normal);
    static void objectMoveCollisionCallback(Object *self, Object *object, Vector2 normal);

    private:
    Vector2 directionMovement;
};

#endif