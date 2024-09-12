#include <player.hpp>
#include <dynamic_ground.hpp>
#include <iostream>
#include <assets.h>
#include <game.hpp>
#include <door.hpp>

DynamicGround::DynamicGround(): Object(){
    this->directionMovement = {0,0};
}

bool DynamicGround::update(){
    string group = this->getGroup();
    if(!group.compare(0,11, "GroundMoveS")){
        applyForceX(0.3*this->directionMovement.x);
        applyForceY(0.3*this->directionMovement.y);
    }else{}

    return Object::update();
}

void DynamicGround::applyCollisionRoles(Object *object, Vector2 normal){
    DynamicGround::invertDirectionCollisionCallback(this, object, normal);
    DynamicGround::objectMoveCollisionCallback(this, object, normal);

    Object::applyCollisionRoles(object, normal);
}

void DynamicGround::setGroup(string group){
    Object::setGroup(group);

    if(!group.compare(0, 10, "GroundMove")){
        if(!group.compare(10,1, "S")){
            if(!group.compare(11,1, "H")){
                this->directionMovement = {1, 0};
            }else if(!group.compare(11,1, "V")){
                this->directionMovement = {0, 1};
            }
        }
    }
}

Vector2 DynamicGround::getDirectionMovement(){
    return this->directionMovement;
}


void DynamicGround::invertDirectionCollisionCallback(Object *self, Object *object, Vector2 normal){
    string group = object->getGroup();

    if(!group.compare(0, 15, "InvertDirection")){
        if(group.find("LH") != string::npos) ((DynamicGround *) self)->directionMovement = {-1, 0};
        if(group.find("RH") != string::npos) ((DynamicGround *) self)->directionMovement = {1, 0};
        if(group.find("UH") != string::npos) ((DynamicGround *) self)->directionMovement = {0, -1};
        if(group.find("DH") != string::npos) ((DynamicGround *) self)->directionMovement = {0, 1};
    }
}

void DynamicGround::objectMoveCollisionCallback(Object *self, Object *object, Vector2 normal){
    if(object->isUsingPhysic()){
        Vector2 direction = ((DynamicGround *) self)->getDirectionMovement();

        std::cout << direction.x << ' ' << direction.y << std::endl;

        if(direction.x || direction.y){
            Vector2 position = object->getPosition();

            if(direction.x) position.x += 0.6*direction.x;
            if(direction.y < 0) position.y += 0.6*direction.y;
            object->setPosition(position.x, position.y);
        }


        // ((Player *)object)->applyForceX(0.3*direction.x);
        // if(direction.y > 0) ((Player *)object)->applyForceY(0.3*abs(direction.y));
    }
}
