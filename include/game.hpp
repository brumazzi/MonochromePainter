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
    void setScore(long int score);
    void setLife(long int life);

    short getLife();
    void gameOver();
    void gameStart();
    void eventEndGame();
    void restartLevel();
    Player *getPlayer();
    long int getScore();
    Stage *getStage();
    Camera2D getCamera();
    vector<string> levelList;
    vector<string> mapList;
    unsigned int mapIndex;
    short lifeCollected;

    void takeConquist(string conquist);
    unsigned int levelIndex;

    private:
    unsigned int saveMsgTimeout;
    Camera2D camera;
    Stage * stage;
    long int score;
    short life;
    bool _gameOver;
    unsigned short stageCount;
    short displayAlpha;
    bool toDark;
    bool isNextLevel;
};

#endif