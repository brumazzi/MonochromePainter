#ifndef __NPC_HPP__
#define __NPC_HPP__

#include <object.hpp>

class NPC: public Object{
    public:
    NPC();

    bool update() override;
    void jump();
    void move(Vector2 direction);
    void setMovementSpeed(float value);
    float getMovementSpeed();
    void setJumpForce(float force);

    private:

    float movementSpeed;
    bool _isJumpEnd;
    float jumpForce;
};

#endif