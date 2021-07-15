#include "cgl/common.h"
#include "cgl/shader.h"
#include "cgl/shader_program.h"
#include "glPong/drawable.h"
#include "glPong/log.h"
#include <glPong/paddle.h>
#include <stdlib.h>

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
        0.5f,  0.5f,  // top right
        0.5f,  -0.5f, // bottom right
        -0.5f, -0.5f, // bottom left
        -0.5f, 0.5f   // top left
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

void PaddleDraw(struct Paddle *p)
{
    // use shader before configuring uniforms
    cgl_shader_program_use(&p->draw->prog);

    // configure uniforms...

    // bind VAO and draw elements and after that unbind
    cgl_vao_bind(&p->draw->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PaddleDelete(struct Paddle *p)
{
    DrawableDelete(p->draw);
    free(p);
}