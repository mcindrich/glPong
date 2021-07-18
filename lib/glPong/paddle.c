#include "cgl/common.h"
#include "cgl/object.h"
#include "cgl/shader.h"
#include "cgl/shader_program.h"
#include "cglm/affine-mat.h"
#include "cglm/affine.h"
#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "cglm/vec4.h"
#include "glPong/direction.h"
#include "glPong/drawable.h"
#include "glPong/log.h"
#include <glPong/paddle.h>
#include <stdlib.h>
#include <glPong/log.h>

struct Paddle *PaddleNew()
{
    struct Paddle *p = NULL;

    p = (struct Paddle *)malloc(sizeof(struct Paddle));
    if (!p)
    {
        return NULL;
    }

    p->draw = DrawableNew();
    if (!p->draw)
    {
        free(p);
        p = NULL;
    }

    return p;
}

int PaddleLoadResources(struct Paddle *p)
{
    int err = 0;

    // load shaders and anything needed to use the paddle
    // also configure vertex data and attributes
    float vertices[] = {
        .5f,  1,  // top right
        .5f,  -1, // bottom right
        -.5f, -1, // bottom left
        -.5f, 1   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // vao
    cgl_vao_gen(&p->draw->vao);
    cgl_vao_bind(&p->draw->vao);

    // ebo
    cgl_ebo_gen(&p->draw->ebo);
    cgl_ebo_bind(&p->draw->ebo);
    cgl_buffer_data(cgl_buffer_kind_element, sizeof(indices), indices, cgl_draw_kind_static);

    // vbo
    cgl_vbo_gen(&p->draw->vbo);
    cgl_vbo_bind(&p->draw->vbo);
    cgl_buffer_data(cgl_buffer_kind_array, sizeof(vertices), vertices, cgl_draw_kind_static);
    cgl_vertex_attrib_configure(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    cgl_vertex_attrib_enable(0);

    // configure shaders
    err = cgl_shader_load_source(&p->draw->vShader, cgl_shader_kind_vertex, "deps/glsl/paddle/VertexShader.glsl");
    if (err)
    {
        LogError("error loading vertex shader source");
        return err;
    }
    err = cgl_shader_load_source(&p->draw->fShader, cgl_shader_kind_fragment, "deps/glsl/paddle/FragmentShader.glsl");
    if (err)
    {
        LogError("error loading fragment shader source");
        return err;
    }

    char errbuffer[512];
    // compile shaders
    err = cgl_shader_compile(&p->draw->vShader);
    cgl_shader_compile_info(&p->draw->vShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling vertex shader");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    err = cgl_shader_compile(&p->draw->fShader);
    cgl_shader_compile_info(&p->draw->fShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling fragment shader");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    // link into a program
    cgl_shader_program_set_vertex_shader(&p->draw->prog, &p->draw->vShader);
    cgl_shader_program_set_fragment_shader(&p->draw->prog, &p->draw->fShader);

    err = cgl_shader_program_link(&p->draw->prog);
    cgl_shader_program_link_info(&p->draw->prog, errbuffer, 512);
    if (err)
    {
        LogError("error linking shader program");
        LogError("linking info: %s", errbuffer);
        return err;
    }
    return err;
}

void PaddleDraw(struct Paddle *p, int neg_pos)
{
    int loc = -1;
    struct cgl_shader_program *prog = &p->draw->prog;
    struct Drawable *draw = p->draw;
    const int w = draw->uResolution[0];
    const int h = draw->uResolution[1];

    // use shader before configuring uniforms
    cgl_shader_program_use(prog);

    // configure uniforms...
    // configure matrices
    glm_mat4_identity(draw->uTranslation);
    glm_mat4_identity(draw->uScaling);
    glm_mat4_identity(draw->uRotation);

    if (neg_pos)
    {
        draw->pos[0] = -((float)w - (float)w / 20);
    }
    else
    {
        draw->pos[0] = ((float)w - (float)w / 20);
    }

    // LogDebug("translating by (%f, %f)", draw->pos[0] / w, draw->pos[1] / h);
    glm_translate(draw->uTranslation, (vec3){draw->pos[0] / w, draw->pos[1] / h, 0});
    glm_rotate(draw->uRotation, draw->rotAngle, (vec3){0, 0, 1});
    glm_scale(draw->uScaling, (vec3){draw->rectSize[0], draw->rectSize[1], 0});
    // glm_ortho(0, w, 0, h, .1f, 100, draw->projectionMatrix);

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uTranslation");
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)draw->uTranslation);
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uRotation");
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)draw->uRotation);
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uScaling");
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)draw->uScaling);
    }

    // bind VAO and draw elements and after that unbind
    cgl_vao_bind(&draw->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PaddleMove(struct Paddle *p, enum Direction dir)
{
    const int w = p->draw->uResolution[0];
    const int h = p->draw->uResolution[1];
    const float incPos = p->draw->speed * h;

    switch (dir)
    {
    case DirectionNone:
        break;
    case DirectionLeft:
        p->draw->rotAngle -= glm_rad(1.5);
        break;
    case DirectionRight:
        p->draw->rotAngle += glm_rad(1.5);
        break;
    case DirectionUp:
        if ((p->draw->pos[1] + (incPos + (p->draw->rectSize[1] / 2) * h)) <= h)
            p->draw->pos[1] += incPos;
        break;
    case DirectionDown:
        if ((p->draw->pos[1] - (incPos + (p->draw->rectSize[1] / 2) * h)) >= -h)
            p->draw->pos[1] -= incPos;
        break;
        break;
    }
}

void PaddleSetInitialPosition(struct Paddle *p, float x, float y)
{
    glm_vec4((vec3){x, y, 0}, 1.0f, p->draw->pos);
}

void PaddleDelete(struct Paddle *p)
{
    DrawableDelete(p->draw);
    free(p);
}