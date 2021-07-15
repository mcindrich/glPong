#ifndef glPong_PADDLE_H
#define glPong_PADDLE_H

#include <glPong/drawable.h>

struct Paddle
{
    struct Drawable *draw;
};

struct Paddle *PaddleNew();
int PaddleLoadResources(struct Paddle *p);
void PaddleDraw(struct Paddle *p);
void PaddleDelete(struct Paddle *p);

#endif // glPong_PADDLE_H