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
    int defaultGamePad = 0;
    bool joysticAvaliable = IsGamepadAvailable(defaultGamePad);
    float axisX = 0.0;
    bool jumpButtom = false;
    bool leftJ = false;
    bool rightJ = false;

    if(joysticAvaliable){
        axisX = GetGamepadAxisMovement(defaultGamePad, GAMEPAD_AXIS_LEFT_X);
        jumpButtom = IsGamepadButtonDown(defaultGamePad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        leftJ = IsGamepadButtonDown(defaultGamePad, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        rightJ = IsGamepadButtonDown(defaultGamePad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    }

    if(IsKeyDown(KEY_LEFT) || (axisX < 0) || leftJ){
        this->applyForceX(-1);
        this->setTexture(getAsset("Animated", "PlayerWalk"), 4);
    }else if(IsKeyDown(KEY_RIGHT) || (axisX > 0) || rightJ){
        this->applyForceX(1);
        this->setTexture(getAsset("Animated", "PlayerWalk"), 4);
    }else{
        this->applyForceX(0);
        this->setTexture(getAsset("Animated", "PlayerIddle"), 1);
    }

    if(this->getLinearVelocity().y > 0.6 && !this->isGrounded()){
        this->setTexture(getAsset("Animated", "PlayerJump"), 1);
    }

    if(IsKeyDown(KEY_X) || IsKeyDown(KEY_SPACE) || jumpButtom || IsKeyDown(KEY_UP)) this->jump();

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
                SetSoundVolume(self->getGame()->soundsAction["item"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["item"]);
            }else if(!group.compare("ItemKey")){
                self->getStage()->getKey();
                SetSoundVolume(self->getGame()->soundsAction["key"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["key"]);
                ((Door *) self->getStage()->getObjects("DoorNext")[0])->open();
            }else if(!group.compare("ItemLife")){
                SetSoundVolume(self->getGame()->soundsAction["life"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["life"]);
                self->getGame()->addLife();
            }else if(!group.compare("ItemChest")){
                SetSoundVolume(self->getGame()->soundsAction["item"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["item"]);
                self->getGame()->addScore(50);
            }else if(!group.compare("ItemPaint")){
                SetSoundVolume(self->getGame()->soundsAction["life"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["life"]);
                self->getGame()->mapIndex++;
                self->getStage()->updateTexture();
            }else if(!group.compare(0, 12,"ItemConquist")){
                SetSoundVolume(self->getGame()->soundsAction["life"], vol+0.4);
                PlaySound(self->getGame()->soundsAction["life"]);
                self->getGame()->takeConquist(group);
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
        SetSoundVolume(self->getGame()->soundsAction["jump"], vol+0.4);
        PlaySound(self->getGame()->soundsAction["jump"]);
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
            SetSoundVolume(self->getGame()->soundsAction["monster-jump"], vol+0.4);
            PlaySound(self->getGame()->soundsAction["monster-jump"]);
            if(IsKeyDown(KEY_X) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))){
                if(object->getGroup().find("Ballon") != std::string::npos){
                    Vector2 pos = object->getPosition();
                    Vector2 selfPos = self->getPosition();
                    self->setPosition(selfPos.x, pos.y-16);
                    self->applyForceY(-6.0);
                }
                else self->applyForceY(-2.5);
            }
            else self->applyForceY(-1.0);
        }
        else if(object->getGroup().find("Nodamage") == std::string::npos && object->getGroup().find("Ballon") == std::string::npos) self->getStage()->tryRespawn();
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

void Player::platformCollisionCallback(Object *self, Object *object, Vector2 normal){
    // string group = object->getGroup();
    // if(!group.compare(0, 8, "Platform")){

    // }
}
