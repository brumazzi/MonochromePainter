#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <raylib.h>
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class Game;
class Stage;

void initTexts();
const char *getText(string group);

class Object{
    public:
    Object();
    virtual ~Object();

    virtual void draw();
    virtual bool update();
    virtual void destroy();
    void jump();

    float getDirection();
    float getLifeTime();
    int getFrameCount();
    unsigned int getFrameDelay();
    Vector2 getPosition();
    Vector2 getLastPosition();
    Vector2 getDimension();
    Texture getTexture();
    Vector2 getTextureOffset();
    Vector2 getTexturePosition();
    Vector2 getTextureScale();
    Rectangle getCollisionBounds();
    Game *getGame();
    Stage *getStage();
    string getGroup();
    Vector2 getLinearVelocity();
    Vector2 getOrigin();

    void setDirection(float direction);
    void setLifeTime(float lifeTime);
    void setAnimated(bool animated);
    void setFrameCount(int frames);
    void setFrameDelay(unsigned int delay);
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setTexture(Texture tex, unsigned int frames);
    void setTextureOffset(float x, float y);
    void setTexturePosition(float x, float y);
    void setTextureScale(float x, float y);
    void setStage(Stage *game);
    void setOrigin(Vector2 origin);
    virtual void setGroup(string group);
    void setCollisionBoundsOffset(Vector2 offset);
    void applyForceY(float force);
    void applyForceX(float force);
    void addCollisionCallback(void (callback)(Object *, Object *, Vector2));

    void usePhysic(bool usePhysic, float worldGravity);
    bool isUsingPhysic();
    virtual Vector2 testCollision(Object *object);
    virtual void applyCollisionRoles(Object *object, Vector2 normal);

    bool isAnimated();
    bool isDestroyed();
    bool isGrounded();
    bool isPlayer();
    bool isSolid();
    void setPlayer(bool isPlayer);
    void setSolid(bool isSolid);

    vector<void (*)(Object *, Object *, Vector2)> collisionCallbacks;

    private:
    float direction;
    int lifeTime;

    bool _isPlayer;
    bool _isDestroyed;
    bool _isGrounded;
    bool _isSolid;
    bool _usePhysic;

    float worldGravity;

    bool _isAnimated;
    unsigned int animationFrameCount;
    unsigned int animationFrame;
    unsigned int animationStep;
    unsigned int animationDelay;
    Vector2 origin;

    Vector2 directionForce;
    Vector2 lastPosition;
    Vector2 position;
    Vector2 size;

    Texture2D texture;
    Vector2 textureOffset;
    Vector2 texturePosition;
    Vector2 textureScale;

    Rectangle collisionBounds;
    Vector2 collisionBoundsOffset;
    Stage *stage;
    string group;

    int windAnimation;
    int windNext;
    int windIndex;
};

#endif