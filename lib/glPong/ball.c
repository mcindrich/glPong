#include "cgl/object.h"
#include "cgl/vao.h"
#include <glPong/ball.h>
#include <glPong/log.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

struct Ball *BallNew()
{
    struct Ball *b = NULL;

    b = malloc(sizeof(struct Ball));
    if (!b)
    {
        return b;
    }

    b->draw = DrawableNew();
    if (!b->draw)
    {
        free(b);
        b = NULL;
    }

    return b;
}

int BallLoadResources(struct Ball *b)
{
    int err = 0;

    float vertices[] = {
        1.f,  1.f,  // top right
        1.f,  -1.f, // bottom right
        -1.f, -1.f, // bottom left
        -1.f, 1.f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // vao
    cgl_vao_gen(&b->draw->vao);
    cgl_vao_bind(&b->draw->vao);

    // vbo
    cgl_vbo_gen(&b->draw->vbo);
    cgl_vbo_bind(&b->draw->vbo);
    cgl_buffer_data(cgl_buffer_kind_array, sizeof(vertices), vertices, cgl_draw_kind_static);
    cgl_vertex_attrib_configure(0, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)), 0);
    cgl_vertex_attrib_enable(0);

    // ebo
    cgl_ebo_gen(&b->draw->ebo);
    cgl_ebo_bind(&b->draw->ebo);
    cgl_buffer_data(cgl_buffer_kind_element, sizeof(indices), indices, cgl_draw_kind_static);

    // configure shaders
    err = cgl_shader_load_source(&b->draw->vShader, cgl_shader_kind_vertex, "deps/glsl/ball/VertexShader.glsl");
    if (err)
    {
        LogError("error loading vertex shader source");
        return err;
    }
    err = cgl_shader_load_source(&b->draw->fShader, cgl_shader_kind_fragment, "deps/glsl/ball/FragmentShader.glsl");
    if (err)
    {
        LogError("error loading fragment shader source");
        return err;
    }

    char errbuffer[512];
    // compile shaders
    err = cgl_shader_compile(&b->draw->vShader);
    cgl_shader_compile_info(&b->draw->vShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling vertex shader for the ball");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    err = cgl_shader_compile(&b->draw->fShader);
    cgl_shader_compile_info(&b->draw->fShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling fragment shader for the ball");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    // link into a program
    cgl_shader_program_set_vertex_shader(&b->draw->prog, &b->draw->vShader);
    cgl_shader_program_set_fragment_shader(&b->draw->prog, &b->draw->fShader);

    err = cgl_shader_program_link(&b->draw->prog);
    cgl_shader_program_link_info(&b->draw->prog, errbuffer, 512);
    if (err)
    {
        LogError("error linking shader program");
        LogError("linking info: %s", errbuffer);
        return err;
    }
    return err;
}

void BallDraw(struct Ball *b)
{
    struct cgl_shader_program *prog = &b->draw->prog;
    int loc = -1;
    // use shader before configuring uniforms
    cgl_shader_program_use(prog);

    // configure uniforms...
    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uTime");
    if (loc != -1)
    {
        glUniform1f(loc, glfwGetTime());
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uResolution");
    if (loc != -1)
    {
        glUniform2f(loc, b->draw->uResolution[0], b->draw->uResolution[1]);
    }

    // bind VAO and draw elements and after that unbind
    cgl_vao_bind(&b->draw->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    cgl_vao_unbind();
}

void BallDelete(struct Ball *b)
{
    DrawableDelete(b->draw);
    free(b);
}