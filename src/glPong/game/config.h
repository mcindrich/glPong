#ifndef glPong_GAME_CONFIG_H
#define glPong_GAME_CONFIG_H

#include <cglm/cglm.h>

typedef struct GameConfig GameConfig;
typedef struct BallConfig BallConfig;
typedef struct PaddleConfig PaddleConfig;

struct BallConfig
{
    float speed;
    vec2 rectSize;
};

struct PaddleConfig
{
    float speed;
    vec2 rectSize;
};

struct GameConfig
{
    BallConfig ballData;
    PaddleConfig paddleData;
    // other game data like background, used styles/themes later on etc.
};

void GameConfigInit(GameConfig *cfg);
int GameConfigLoad(GameConfig *cfg, const char *path);
int GameConfigExport(GameConfig *cfg, const char *path);
void GameConfigDelete(GameConfig *cfg);

#endif // glPong_GAME_CONFIG_H