#ifndef glPong_BALL_H
#define glPong_BALL_H

#include <glPong/paddle.h>
#include <glPong/drawable.h>
#include <stdint.h>

typedef struct Ball Ball;

struct Ball
{
    Drawable *draw;      // drawable interface for a ball
    Direction direction; // indicates direction of the ball (left or right)
    vec2 dirUnitVec;     // direction unit vector -> initially set to left or right (1, 0) or (-1, 0)
    float radius;        // ball circle radius
};

Ball *BallNew();
int BallLoadResources(Ball *b);
// functions for checking collision - if collision occurs, functions change vectors and directions of the ball
int BallCheckWallCollision(Ball *b);
void BallCheckPaddleCollision(Ball *b, Paddle *p);
void BallDraw(Ball *b);
void BallDelete(Ball *b);

#endif // glPong_BALL_H