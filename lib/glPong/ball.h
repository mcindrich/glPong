#ifndef glPong_BALL_H
#define glPong_BALL_H

#include <glPong/drawable.h>

struct Ball
{
    struct Drawable *draw;
};

struct Ball *BallNew();
int BallLoadResources(struct Ball *b);
void BallDraw(struct Ball *b);
void BallDelete(struct Ball *b);

#endif // glPong_BALL_H