#include <object.hpp>
#include <game.hpp>
#include <stage.hpp>
#include <iostream>

static map<string, const char*> textList;
void initTexts(){
    textList["TextWALL_WALK"] = "Use \"Wall Walk\" to climb wall!";
}
const char *getText(string group){
    return textList[group];
}

Object::Object(){
    this->direction = 0;
    this->lifeTime = -1;
    this->_isPlayer = false;

    this->_usePhysic = false;
    this->_isDestroyed = false;
    this->_isGrounded = false;
    this->worldGravity = 0.0;
    this->directionForce = {0.0, 0.0};

    this->_isAnimated = false;
    this->animationFrameCount = 1;
    this->animationFrame = 0;
    this->animationStep = 0;
    this->animationDelay = 0;

    this->lastPosition = {0,0};
    this->position = {0,0};
    this->size = {0,0};

    this->texture.id = 0;
    this->textureOffset = {0,0};
    this->texturePosition = {0,0};
    this->textureScale = {1,1};

    this->collisionBounds = {0,0,0,0};
    this->collisionBoundsOffset = {0,0};
    this->stage = nullptr;
}
Object::~Object(){
    this->collisionCallbacks.clear();
}

void Object::draw(){
    if(this->_isDestroyed) return;

    if(!this->group.compare(0, 4, "Text")){
        const char *text = getText(this->group);
        int textSize = MeasureText(text, 6);
        DrawText(text, (this->position.x+8) - (textSize/2), this->position.y-16, 6, WHITE);
        return;
    }

    if(this->texture.id != 0){
        DrawTexturePro(
            this->texture,
            (Rectangle) {
                (this->textureOffset.x*this->size.x)+(this->animationFrame*this->size.x),
                (this->textureOffset.y*this->size.y),
                this->size.x * this->textureScale.x,
                this->size.y * this->textureScale.y
            },
            (Rectangle) {
                this->position.x,
                this->position.y,
                this->size.x,
                this->size.y
            },
            (Vector2) {
                this->texturePosition.x,
                this->texturePosition.y,
            },
            this->direction,
            WHITE
        );
    }

    // DrawRectangleLinesEx(this->collisionBounds, 1, WHITE);
}

bool Object::update(){
    if(this->_isDestroyed) return false;

    unsigned int fps = GetFPS();
    if(!fps) return false;

    if(this->animationDelay > 0){
        this->animationStep = (this->animationStep+1)%this->animationDelay;
        if(this->animationStep == 0){
            this->animationFrame = (this->animationFrame+1)%this->animationFrameCount;
        }
    }

    if(this->lifeTime > -1){
        if(this->lifeTime == 0){
            this->_isDestroyed = true;
        }else this->lifeTime--;
    }

    if(this->_usePhysic){
        Vector2 position = this->getPosition();
        position.y += this->directionForce.y;
        position.x += this->directionForce.x;
        this->setPosition(position.x, position.y);
        this->directionForce.y += this->worldGravity/fps;
    }

    this->collisionBounds = (Rectangle) {
        this->position.x + this->collisionBoundsOffset.x,
        this->position.y + this->collisionBoundsOffset.y,
        this->size.x - + this->collisionBoundsOffset.x*2,
        this->size.y - + this->collisionBoundsOffset.y*2
    };

    if(this->getLinearVelocity().y != 0) this->_isGrounded = false;

    return true;
}


float Object::getDirection(){ return this->direction; }
float Object::getLifeTime(){ return this->lifeTime; }
int Object::getFrameCount(){ return this->animationFrameCount; }
unsigned int Object::getFrameDelay(){return this->animationDelay; }
Vector2 Object::getPosition(){ return this->position; }
Vector2 Object::getLastPosition(){ return this->lastPosition; }
Vector2 Object::getDimension(){ return this->size; }
Texture Object::getTexture(){ return this->texture; }
Vector2 Object::getTextureOffset(){ return this->textureOffset; }
Vector2 Object::getTexturePosition(){ return this->texturePosition; }
Vector2 Object::getTextureScale(){ return this->textureScale; }
Game *Object::getGame(){ return this->stage->getGame(); }
Stage *Object::getStage(){ return this->stage; }
Rectangle Object::getCollisionBounds(){ return this->collisionBounds; }
string Object::getGroup(){ return this->group; }
Vector2 Object::getLinearVelocity(){ return (Vector2) {position.x - lastPosition.x, position.y - lastPosition.y}; }


void Object::setDirection(float direction){ this->direction; }
void Object::setLifeTime(float lifeTime){ this->lifeTime; }
void Object::setAnimated(bool animated){ this->_isAnimated = animated; }
void Object::setFrameCount(int frames){ this->animationFrameCount = frames; }
void Object::setFrameDelay(unsigned int delay){ this->animationDelay = delay; }
void Object::setPosition(float x, float y){
    this->lastPosition = {position.x, position.y};
    this->position = {x,y};
}
void Object::setSize(float w, float h){ this->size = {w, h}; }
void Object::setTexture(Texture tex, unsigned int frames){ this->texture = tex; this->animationFrameCount = frames; }
void Object::setTextureOffset(float x, float y){ this->textureOffset = {x, y}; }
void Object::setTexturePosition(float x, float y){ this->texturePosition = {x, y}; }
void Object::setTextureScale(float x, float y){ this->textureScale = {x, y}; }
void Object::setCollisionBoundsOffset(Vector2 offset){ this->collisionBoundsOffset = offset; }
void Object::setStage(Stage *stage){ this->stage = stage; }
void Object::setGroup(string group){
    this->group = group;
    if(group.find("MirrorY") != std::string::npos){
        this->textureScale = {1, -1};
    }else if(group.find("MirrorX") != std::string::npos){
        this->textureScale = {-1, 1};
    }else this->textureScale = {1, 1};
}
void Object::applyForceY(float force){
    this->directionForce.y = force;
    this->_isGrounded = false;
}
void Object::applyForceX(float force){
    this->directionForce.x = force;
}
void Object::destroy(){ this->_isDestroyed = true; }

void Object::usePhysic(bool usePhysic, float worldGravity){
    this->_usePhysic = usePhysic;
    this->worldGravity = worldGravity;
}
Vector2 Object::testCollision(Object *object){
    if(!this->_usePhysic) return {0,0};

    Rectangle tBounds = object->getCollisionBounds();
    Vector2 offset = this->collisionBoundsOffset;
    Vector2 normal = {0,0};

    if(CheckCollisionRecs(this->getCollisionBounds(), tBounds)){
        if((this->position.x-offset.x+this->size.x) > tBounds.x && (this->lastPosition.x-offset.x+this->size.x) <= tBounds.x){
            normal.x = -1;
        }else if((this->position.x+offset.x) < (tBounds.x+tBounds.width) && (this->lastPosition.x+offset.x) >= (tBounds.x+tBounds.width)){
            normal.x = 1;
        }
        if((this->position.y-offset.y+this->size.y) > tBounds.y && (this->lastPosition.y-offset.y+this->size.y) <= tBounds.y){
            normal.y = -1;
        }else if((this->position.y+offset.y) < (tBounds.y+tBounds.height) && (this->lastPosition.y+offset.y) >= (tBounds.y+tBounds.height)){
            normal.y = 1;
        }

        for(const auto callback: this->collisionCallbacks){
            callback(this, object, normal);
        }

        this->applyCollisionRoles(object, normal);
    }

    return normal;
}
void Object::applyCollisionRoles(Object *object, Vector2 normal){
    if(!object->isSolid()) return;
    if(normal.x == 0 && normal.y == 0) return;

    Rectangle bounds = object->getCollisionBounds();
    Vector2 offset = this->collisionBoundsOffset;

    if(object->getGroup().compare(0, 4, "Wall")){
        if(normal.y == -1){
            this->_isGrounded = true;
            this->directionForce.y = 0.0;
            this->position.y = bounds.y - this->size.y + offset.y;
            this->lastPosition.y = bounds.y - this->size.y + offset.y;
        }else if(normal.y == 1){
            if(!object->getGroup().compare(0, 4, "Platform")){
                this->position.y = (bounds.y + bounds.height - offset.y);
                this->lastPosition.y = (bounds.y + bounds.height - offset.y);
                this->directionForce.y *= -0.4;
            }
        }
    }

    if(normal.x == -1){
        this->position.x = bounds.x - this->size.x + offset.x;
        this->lastPosition.x = bounds.x - this->size.x + offset.x;
    }else if(normal.x == 1){
        this->position.x = (bounds.x + bounds.width - offset.x);
        this->lastPosition.x = (bounds.x + bounds.width - offset.x);
    }
}

bool Object::isUsingPhysic(){ return this->_usePhysic; }
bool Object::isAnimated(){ return this->_isAnimated; }
bool Object::isDestroyed(){ return this->_isDestroyed; }
bool Object::isGrounded(){ return this->_isGrounded; }
bool Object::isPlayer(){ return this->_isPlayer; }
bool Object::isSolid(){ return this->_isSolid; }

void Object::setPlayer(bool isPlayer){ this->_isPlayer = isPlayer; }
void Object::setSolid(bool isSolid){ this->_isSolid = isSolid; }

void Object::addCollisionCallback(void (callback)(Object *, Object *, Vector2)){
    this->collisionCallbacks.push_back(callback);
}
