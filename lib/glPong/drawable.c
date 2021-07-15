#include "cgl/shader.h"
#include "cgl/shader_program.h"
#include "cgl/vbo.h"
#include <glPong/drawable.h>

struct Drawable *DrawableNew()
{
    struct Drawable *d = NULL;

    d = malloc(sizeof(struct Drawable));

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

    glm_vec2((vec2){0, 0}, d->pos);

    return d;
}

void DrawableDelete(struct Drawable *d)
{
    if (d)
    {
        cgl_vbo_delete(&d->vbo);
        cgl_ebo_delete(&d->ebo);
        cgl_vao_delete(&d->vao);

        cgl_shader_delete(&d->vShader);
        cgl_shader_delete(&d->fShader);
        cgl_shader_program_delete(&d->prog);

        free(d);
    }
}