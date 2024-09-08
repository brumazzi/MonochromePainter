#include <item.hpp>
#include <game.hpp>
#include <door.hpp>

Item::Item(): Object(){
    this->setCollisionBoundsOffset({5,5});
}

#include <iostream>
Item::~Item(){
    std::string group = this->getGroup();
    if(!group.compare("ItemPoint")){
        this->getGame()->addScore(5);
    }else if(!group.compare("ItemKey")){
        this->getStage()->getKey();
        ((Door *) this->getStage()->getObjects("DoorNext")[0])->open();
    }else if(!group.compare("ItemLife")){
        this->getGame()->addLife();
    }
}