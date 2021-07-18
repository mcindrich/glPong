#ifndef glPong_BALL_H
#define glPong_BALL_H

#include <glPong/drawable.h>
#include <stdint.h>

struct Ball
{
    struct Drawable *draw; // drawable interface for a ball
    int64_t speed;         // speed at which the ball is going - negative for going left, positive for right
};

struct Ball *BallNew();
int BallLoadResources(struct Ball *b);
void BallDraw(struct Ball *b);
void BallDelete(struct Ball *b);

#endif // glPong_BALL_H