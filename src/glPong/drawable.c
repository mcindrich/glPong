#include "cgl/shader.h"
#include "cgl/shader_program.h"
#include "cgl/vbo.h"
#include "cglm/mat4.h"
#include "cglm/vec2.h"
#include <glPong/drawable.h>

Drawable *DrawableNew()
{
    Drawable *d = NULL;

    d = malloc(sizeof(Drawable));

    if (!d)
    {
        return NULL;
    }

    // init all
    cgl_vbo_init(&d->vbo);
    cgl_ebo_init(&d->ebo);
    cgl_vao_init(&d->vao);

    cgl_shader_init(&d->vShader);
    cgl_shader_init(&d->fShader);
    cgl_shader_program_init(&d->prog);

    glm_vec4((vec3){0, 0, 0}, 0, d->pos);

    d->speed = 0;
    d->rotAngle = 0;

    return d;
}

void DrawableSetSpeed(Drawable *d, float s)
{
    d->speed = s;
}

void DrawableSetDefaults(Drawable *d)
{
    glm_vec4((vec3){0, 0, 0}, 0, d->pos);
}

void DrawableSetRectSize(Drawable *d, vec2 s)
{
    glm_vec2_copy(s, d->rectSize);
}

void DrawableDelete(Drawable *d)
{
    if (d)
    {
        cgl_vbo_delete(&d->vbo);
        cgl_ebo_delete(&d->ebo);
        cgl_vao_delete(&d->vao);

        cgl_shader_delete(&d->vShader);
        cgl_shader_delete(&d->fShader);
        cgl_shader_program_delete(&d->prog);
    }
}