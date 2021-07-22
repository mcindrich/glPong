#ifndef glPong_PADDLE_H
#define glPong_PADDLE_H

#include <glPong/drawable.h>
#include <glPong/direction.h>

struct Paddle
{
    struct Drawable *draw;
};

struct Paddle *PaddleNew();
int PaddleLoadResources(struct Paddle *p);
void PaddleDraw(struct Paddle *p, enum Direction side);
void PaddleMove(struct Paddle *p, enum Direction up);
void PaddleSetInitialPosition(struct Paddle *p, float x, float y);
void PaddleDelete(struct Paddle *p);

#endif // glPong_PADDLE_H