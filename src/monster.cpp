#include <monster.hpp>
#include <stage.hpp>
#include <iostream>

Monster::Monster(): NPC(){
    this->directionMovement = (Vector2) {0,0};
    this->setMovementSpeed(0.8);
    this->setCollisionBoundsOffset({2,4});
    this->setFrameDelay(10);
}

bool Monster::update(){
    if(this->directionMovement.x != 0){
        this->applyForceX(this->getMovementSpeed()*this->directionMovement.x);
        this->applyForceY(0);
    }else if(this->directionMovement.y != 0){
        this->applyForceX(0);
        this->applyForceY(this->getMovementSpeed()*this->directionMovement.y);
    }

    if(!NPC::update()) return false;
    return true;
}

void Monster::setGroup(string group){
    Object::setGroup(group);

    if(this->getGroup().find("Stop") != std::string::npos){
        this->directionMovement = {0, 0};
        return;
    }

    if(group.find("Walk") != string::npos){
        this->directionMovement.x = 1;
    }else if(group.find("Fly") != string::npos || group.find("Spider") != string::npos){
        this->directionMovement.y = 1;
    }else if(group.find("Ballon") != string::npos){
        this->setCollisionBoundsOffset({0,2});
        this->directionMovement.y = 1;
        this->setMovementSpeed(0.0);
    }
}

void Monster::applyCollisionRoles(Object *object, Vector2 normal){
    Monster::directionCollisionCallback(this, object, normal);
    Monster::playerCollisionCallback(this, object, normal);

    Object::applyCollisionRoles(object, normal);
}

void Monster::playerCollisionCallback(Object *self, Object *object, Vector2 normal){
    string group = object->getGroup();
    Monster *monster = (Monster *) self;
    if(monster->getGroup().find("Ballon") != string::npos && !group.compare(0, 6, "Player")){
        if(monster->getMovementSpeed() == 0.0){
            monster->setMovementSpeed(0.5);
            for(auto oMonster: self->getStage()->getObjects("Monster")){
                Vector2 pos = oMonster->getOrigin();
                oMonster->setPosition(pos.x, pos.y);
            }
        }
    }
}

void Monster::directionCollisionCallback(Object *self, Object *object, Vector2 normal){
    string group = object->getGroup();
    if(self->getGroup().find("Stop") != std::string::npos){
        ((Monster *) self)->directionMovement = {0, 0};
        return;
    }

    if(!group.compare(0, 15, "InvertDirection")){
        if(group.find("LH") != string::npos) ((Monster *) self)->directionMovement = {-1, 0};
        if(group.find("RH") != string::npos) ((Monster *) self)->directionMovement = {1, 0};
        if(group.find("UH") != string::npos) ((Monster *) self)->directionMovement = {0, -1};
        if(group.find("DH") != string::npos) ((Monster *) self)->directionMovement = {0, 1};
    }
}
