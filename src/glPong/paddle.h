#ifndef glPong_PADDLE_H
#define glPong_PADDLE_H

#include <glPong/drawable.h>
#include <glPong/direction.h>

typedef struct Paddle Paddle;

struct Paddle
{
    Drawable *draw;
};

Paddle *PaddleNew();
int PaddleLoadResources(Paddle *p);
void PaddleDraw(Paddle *p, Direction side);
void PaddleMove(Paddle *p, Direction up);
void PaddleDelete(Paddle *p);

#endif // glPong_PADDLE_H