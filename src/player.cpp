#include <player.hpp>
#include <iostream>
#include <assets.h>
#include <game.hpp>
#include <door.hpp>

Player::Player(): NPC(){
    this->setMovementSpeed(4);
    this->setJumpForce(-2.5);
    this->setAnimated(true);
    this->setFrameCount(4);
    this->setFrameDelay(10);
    this->setCollisionBoundsOffset({4,0});
}

bool Player::update(){

    if(IsKeyDown(KEY_LEFT)){
        this->applyForceX(-1);
        this->setTexture(getAsset("Animated", "PlayerWalk"), 4);
    }else if(IsKeyDown(KEY_RIGHT)){
        this->applyForceX(1);
        this->setTexture(getAsset("Animated", "PlayerWalk"), 4);
    }else{
        this->applyForceX(0);
        this->setTexture(getAsset("Animated", "PlayerIddle"), 1);
    }

    if(this->getLinearVelocity().y != 0){
        this->setTexture(getAsset("Animated", "PlayerJump"), 1);
    }

    if(IsKeyDown(KEY_X)) this->jump();
    return NPC::update();
}

void Player::applyCollisionRoles(Object *object, Vector2 normal){
    Player::itemCollisionCallback(this, object, normal);
    Player::respawnCollisionCallback(this, object, normal);
    Player::jumpCollisionCallback(this, object, normal);
    Player::monsterCollisionCallback(this, object, normal);
    Player::nextLevelCollisionCallback(this, object, normal);
    Player::damageCollisionCallback(this, object, normal);
    Player::windCollisionCallback(this, object, normal);

    Object::applyCollisionRoles(object, normal);
}

void Player::itemCollisionCallback(Object *self, Object *object, Vector2 normal){
    std::string group = object->getGroup();

    if(!group.compare(0, 4, "Item")){
        if(group.compare(0, 8, "ItemJump")){
            if(object->isDestroyed()) return;

            std::string group = object->getGroup();
            if(!group.compare("ItemPoint")){
                self->getGame()->addScore(5);
            }else if(!group.compare("ItemKey")){
                self->getStage()->getKey();
                ((Door *) self->getStage()->getObjects("DoorNext")[0])->open();
            }else if(!group.compare("ItemLife")){
                self->getGame()->addLife();
            }else if(!group.compare("ItemPaint")){
                self->getGame()->mapIndex++;
                self->getStage()->updateTexture();
            }
            object->destroy();
        }
    }
}

void Player::respawnCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(!object->getGroup().compare("Respawn")){
        self->getStage()->setRespawn(object);
        object->destroy();
    }
}

void Player::jumpCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(!object->getGroup().compare(0, 8, "ItemJump") && normal.y == -1){
        self->applyForceY(-6.5);
    }
}

void Player::windCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(!object->getGroup().compare(0, 6, "WindUp")){
        self->applyForceY(-2.0);
    }
}

void Player::monsterCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(!object->getGroup().compare(0, 7, "Monster")){
        if(normal.y == -1){
            if(IsKeyDown(KEY_X)) self->applyForceY(-2.0);
            else self->applyForceY(-1.0);
        }
        else self->getStage()->tryRespawn();
    }
}

void Player::nextLevelCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(!object->getGroup().compare("DoorNext")){
        if(((Door*) object)->isOpen()) self->getGame()->nextLevel();
    }
}

void Player::damageCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(object->getGroup().find("Damage") != std::string::npos){
        self->getStage()->tryRespawn();
    }
}
