#ifndef glPong_DRAWABLE_H
#define glPong_DRAWABLE_H

#include <cgl.h>
#include <cglm/cglm.h>

struct Drawable
{
    struct cgl_vbo vbo;
    struct cgl_ebo ebo;
    struct cgl_vao vao;

    struct cgl_shader vShader;
    struct cgl_shader fShader;
    struct cgl_shader_program prog;

    vec2 pos;
};

struct Drawable *DrawableNew();
void DrawableDelete(struct Drawable *d);

#endif // glPong_DRAWABLE_H