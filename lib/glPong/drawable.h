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

    vec4 pos;
    vec4 prevPos;

    mat4 uTranslation;
    mat4 uRotation;
    mat4 uScaling;

    vec2 uResolution;

    float speed;
    vec2 rectSize; // object rectangle size (x, y)

    float rotAngle; // rotation angle
};

struct Drawable *DrawableNew();
void DrawableSetSpeed(struct Drawable *d, float s);
void DrawableSetRectSize(struct Drawable *d, vec2 s);
void DrawableDelete(struct Drawable *d);

#endif // glPong_DRAWABLE_H