#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <raylib.h>
#include <string>
#include <vector>
#include <map>
#include <object.hpp>
#include <stage.hpp>

using std::map;
using std::vector;
using std::string;

#define CAMERA_WIDTH 384
#define CAMERA_HEIGHT 216

class Game{
    public:
    Game();
    ~Game();

    void draw();
    void update();
    void setStage(Stage *stage);
    void addScore(long int point);
    void addLife();
    void subLife();
    void nextLevel();
    bool isGameOver();

    short getLife();
    void gameOver();
    void gameStart();
    Player *getPlayer();
    long int getScore();
    Stage *getStage();
    Camera2D getCamera();
    vector<string> levelList;

    private:
    Camera2D camera;
    Stage * stage;
    long int score;
    short life;
    bool _gameOver;
    unsigned short stageCount;
    short displayAlpha;
    bool toDark;
    bool isNextLevel;
    unsigned int levelIndex;
};

#endif