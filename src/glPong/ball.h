#ifndef glPong_BALL_H
#define glPong_BALL_H

#include <glPong/paddle.h>
#include <glPong/drawable.h>
#include <stdint.h>

struct Ball
{
    struct Drawable *draw;    // drawable interface for a ball
    enum Direction direction; // indicates direction of the ball (left or right)
    vec2 dirUnitVec;          // direction unit vector -> initially set to left or right (1, 0) or (-1, 0)
    float radius;             // ball circle radius
};

struct Ball *BallNew();
int BallLoadResources(struct Ball *b);
// functions for checking collision - if collision occurs, functions change vectors and directions of the ball
int BallCheckWallCollision(struct Ball *b);
void BallCheckPaddleCollision(struct Ball *b, struct Paddle *p);
void BallDraw(struct Ball *b);
void BallDelete(struct Ball *b);

#endif // glPong_BALL_H