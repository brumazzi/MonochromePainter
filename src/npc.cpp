#include <npc.hpp>
#include <iostream>
#include <game.hpp>

NPC::NPC(): Object(){
    this->movementSpeed = 0.0;
    this->_isJumpEnd = false;
    this->jumpForce = 0.0;
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

    if(!this->getGroup().compare("NPC:JUMP_IF_COMPLETE")){
        this->jumpForce = -1.5;
        this->jump();
        this->setTextureScale(1, 1);
        if(this->getGame()->mapIndex < 10) this->destroy();
    }

    return Object::update();

}
void NPC::jump(){
    if(!this->isGrounded()) return;

    this->applyForceY(this->jumpForce);
}

void NPC::setMovementSpeed(float value){ this->movementSpeed = value; }

void NPC::setJumpForce(float force){ this->jumpForce = force; }
float NPC::getMovementSpeed(){ return this->movementSpeed; }