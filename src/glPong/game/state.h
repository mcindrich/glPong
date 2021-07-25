#ifndef glPong_GAME_STATE_H
#define glPong_GAME_STATE_H

enum GameState
{
    GameStateMenu = 0,
    GameStateCountdown,
    GameStatePlaying,
    GameStateOver,
};

typedef enum GameState GameState;

#endif // glPong_GAME_STATE_H