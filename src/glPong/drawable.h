#ifndef glPong_DRAWABLE_H
#define glPong_DRAWABLE_H

#include <cgl.h>
#include <cglm/cglm.h>

typedef struct Drawable Drawable;

struct Drawable
{
    struct cgl_vbo vbo;
    struct cgl_ebo ebo;
    struct cgl_vao vao;

    struct cgl_shader vShader;
    struct cgl_shader fShader;
    struct cgl_shader_program prog;

    vec4 pos;

    mat4 uTranslation;
    mat4 uRotation;
    mat4 uScaling;

    vec2 uResolution;

    float speed;
    vec2 rectSize; // object rectangle size (x, y)

    float rotAngle; // rotation angle
};

Drawable *DrawableNew();
void DrawableInit(Drawable *d);
void DrawableSetSpeed(Drawable *d, float s);
void DrawableSetDefaults(Drawable *d);
void DrawableSetRectSize(Drawable *d, vec2 s);
void DrawableDelete(Drawable *d);

#endif // glPong_DRAWABLE_H