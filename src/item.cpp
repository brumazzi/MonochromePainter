#include <item.hpp>
#include <game.hpp>
#include <door.hpp>

Item::Item(): Object(){
    this->setCollisionBoundsOffset({5,5});
}