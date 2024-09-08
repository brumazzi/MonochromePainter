#include <assets.h>
#include <door.hpp>
#include <object.hpp>
#include <stage.hpp>

Door::Door(): Object(){
    this->_isOpen = false;
}

void Door::draw(){
    if(!this->_isOpen) this->setTexture(getAsset("Static", "DoorClose"), 1);
    else this->setTexture(getAsset("Static", "DoorOpen"), 1);

    Object::draw();
}

void Door::open(){
    this->_isOpen = true;
}
void Door::close(){
    this->_isOpen = false;
}
bool Door::isOpen(){
    return this->_isOpen;
}