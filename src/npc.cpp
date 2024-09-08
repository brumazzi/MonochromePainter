#include <npc.hpp>
#include <iostream>

NPC::NPC(): Object(){
    this->movementSpeed = 0.0;
    this->jumpForce = 0.0;
    this->_isJumpEnd = false;
}

bool NPC::update(){
    Vector2 velocity = this->getLinearVelocity();
    Vector2 textureScale = this->getTextureScale();;
    if(velocity.x < 0){
        textureScale.x = -1;
    }else if(velocity.x > 0){
        textureScale.x = 1;
    }
    this->setTextureScale(textureScale.x, textureScale.y);

    return Object::update();
}
void NPC::jump(){
    if(!this->isGrounded()) return;

    this->applyForceY(this->jumpForce);
}

void NPC::setMovementSpeed(float value){ this->movementSpeed = value; }

void NPC::setJumpForce(float force){ this->jumpForce = force; }
float NPC::getMovementSpeed(){ return this->movementSpeed; }