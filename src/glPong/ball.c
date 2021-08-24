#include "cgl/object.h"
#include "cgl/vao.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/vec2.h"
#include "glPong/direction.h"
#include <glPong/ball.h>
#include <glPong/log.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#define rand_between(min, max) ((min) + (max - min) * ((float)rand() / (float)RAND_MAX))

void BallInit(Ball *b)
{
    DrawableInit(&b->draw);
    b->direction = DirectionNone;
    b->radius = 0;
    glm_vec2_copy((vec2){0, 0}, b->dirUnitVec);
}

int BallLoadResources(Ball *b)
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
    cgl_vao_gen(&b->draw.vao);
    cgl_vao_bind(&b->draw.vao);

    // vbo
    cgl_vbo_gen(&b->draw.vbo);
    cgl_vbo_bind(&b->draw.vbo);
    cgl_buffer_data(cgl_buffer_kind_array, sizeof(vertices), vertices, cgl_draw_kind_static);
    cgl_vertex_attrib_configure(0, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)), 0);
    cgl_vertex_attrib_enable(0);

    // ebo
    cgl_ebo_gen(&b->draw.ebo);
    cgl_ebo_bind(&b->draw.ebo);
    cgl_buffer_data(cgl_buffer_kind_element, sizeof(indices), indices, cgl_draw_kind_static);

    // configure shaders
    err = cgl_shader_load_source(&b->draw.vShader, cgl_shader_kind_vertex, "deps/glsl/ball/VertexShader.glsl");
    if (err)
    {
        LogError("error loading vertex shader source");
        return err;
    }
    err = cgl_shader_load_source(&b->draw.fShader, cgl_shader_kind_fragment, "deps/glsl/ball/FragmentShader.glsl");
    if (err)
    {
        LogError("error loading fragment shader source");
        return err;
    }

    char errbuffer[512];
    // compile shaders
    err = cgl_shader_compile(&b->draw.vShader);
    cgl_shader_compile_info(&b->draw.vShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling vertex shader for the ball");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    err = cgl_shader_compile(&b->draw.fShader);
    cgl_shader_compile_info(&b->draw.fShader, errbuffer, 512);
    if (err)
    {
        LogError("error compiling fragment shader for the ball");
        LogError("compilation info: %s", errbuffer);
        return err;
    }

    // link into a program
    cgl_shader_program_set_vertex_shader(&b->draw.prog, &b->draw.vShader);
    cgl_shader_program_set_fragment_shader(&b->draw.prog, &b->draw.fShader);

    err = cgl_shader_program_link(&b->draw.prog);
    cgl_shader_program_link_info(&b->draw.prog, errbuffer, 512);
    if (err)
    {
        LogError("error linking shader program");
        LogError("linking info: %s", errbuffer);
        return err;
    }
    return err;
}

int BallCheckWallCollision(Ball *b)
{
    const int w = b->draw.uResolution[0], h = b->draw.uResolution[1];
    vec2 left = {-w, 0}, right = {w, 0}, up = {0, h}, down = {0, -h};
    vec2 pos;

    glm_vec2_copy(b->draw.pos, pos);

    const float distLeft = fabs(-w - pos[0]);
    const float distRight = fabs(w - pos[0]);
    const float distUp = fabs(h - pos[1]);
    const float distDown = fabs(-h - pos[1]);
    const float rectW = b->draw.rectSize[0], rectH = b->draw.rectSize[1];
    const float rectLen = sqrt(pow(rectW, 2) + pow(rectH, 2));

    const float speed = b->draw.speed;

    // LogDebug("left, right = %f, %f", distLeft, distRight);

    if (distLeft < rectLen)
    {
        // end game -> right wins
        // LogDebug("end game -> right wins");
        return 1;
    }
    else if (distRight < rectLen)
    {
        // end game -> left wins
        // LogDebug("end game -> left wins");
        return 1;
    }
    else if (distUp < rectLen)
    {
        b->dirUnitVec[0] *= 1;
        b->dirUnitVec[1] *= -1;
    }
    else if (distDown < rectLen)
    {
        b->dirUnitVec[0] *= 1;
        b->dirUnitVec[1] *= -1;
    }
    return 0;
}

int BallCheckPaddleCollision(Ball *b, Paddle *p)
{
    int collides = 1;
    vec2 upper, lower, middle;
    mat4 rotMat;
    vec4 tmp = {0};

    const float third = p->draw.rectSize[1] / 3;

    glm_vec2_copy(p->draw.pos, middle);
    glm_vec2((vec2){p->draw.pos[0], p->draw.pos[1] + third}, upper);
    glm_vec2((vec2){p->draw.pos[0], p->draw.pos[1] - third}, lower);

    const float distMiddle = glm_vec2_distance(b->draw.pos, middle);
    const float distUpper = glm_vec2_distance(b->draw.pos, upper);
    const float distLower = glm_vec2_distance(b->draw.pos, lower);
    const float rectW = b->draw.rectSize[0], rectH = b->draw.rectSize[1];
    const float rectLen = sqrt(pow(rectW, 2) + pow(rectH, 2)) * 2;

    glm_mat4_identity(rotMat);

    if (distMiddle < rectLen)
    {
        // ball collides with the middle of the paddle -> move ball straight
        // LogDebug("ball hit middle");
        if (b->draw.speed < 0)
        {
            b->direction = DirectionRight;
        }
        else
        {
            b->direction = DirectionLeft;
        }
        glm_vec2_copy((vec2){1, 0}, b->dirUnitVec);
        b->draw.speed *= -1;
    }
    else if (distUpper < rectLen)
    {
        // LogDebug("ball hit upper");
        // ball collides with the upper part of the paddle -> move ball down with rand() angle
        if (b->draw.speed < 0)
        {
            b->direction = DirectionRight;
            glm_rotate(rotMat, -glm_rad(rand_between(20, 70)), (vec3){0, 0, 1});
        }
        else
        {
            b->direction = DirectionLeft;
            glm_rotate(rotMat, glm_rad(rand_between(20, 70)), (vec3){0, 0, 1});
        }
        glm_vec2_copy((vec2){1, 0}, tmp);
        glm_mat4_mulv(rotMat, tmp, tmp);
        glm_vec2_copy(tmp, b->dirUnitVec);
        b->draw.speed *= -1;
    }
    else if (distLower < rectLen)
    {
        // LogDebug("ball hit lower");
        // ball collides with the lower part of the paddle -> move ball up with rand() angle
        if (b->draw.speed < 0)
        {
            b->direction = DirectionRight;
            glm_rotate(rotMat, glm_rad(rand_between(20, 70)), (vec3){0, 0, 1});
        }
        else
        {
            b->direction = DirectionLeft;
            glm_rotate(rotMat, -glm_rad(rand_between(20, 70)), (vec3){0, 0, 1});
        }
        glm_vec2_copy((vec2){1, 0}, tmp);
        glm_mat4_mulv(rotMat, tmp, tmp);
        glm_vec2_copy(tmp, b->dirUnitVec);
        b->draw.speed *= -1;
    }
    else
    {
        collides = 0;
    }

    return collides;
}

void BallDraw(Ball *b)
{
    Drawable *draw = &b->draw;
    struct cgl_shader_program *prog = &draw->prog;
    int loc = -1;
    const unsigned int w = draw->uResolution[0];
    const unsigned int h = draw->uResolution[1];

    // use shader before configuring uniforms
    cgl_shader_program_use(prog);

    if (b->direction == DirectionLeft && draw->speed > 0)
    {
        // going left -> negative
        draw->speed *= -1;
    }

    // LogDebug("speed = %f", draw->speed);
    // LogDebug("unitVec = %f, %f", b->dirUnitVec[0], b->dirUnitVec[1]);

    draw->pos[0] += b->dirUnitVec[0] * draw->speed * b->deltaTime * draw->uResolution[0];
    draw->pos[1] += b->dirUnitVec[1] * draw->speed * b->deltaTime * draw->uResolution[1];
    // draw->pos[1] = 70;

    // LogDebug("(x, y) = (%f, %f)", draw->pos[0], draw->pos[1]);

    glm_mat4_identity(draw->uTranslation);
    glm_mat4_identity(draw->uScaling);
    glm_mat4_identity(draw->uRotation);

    glm_translate(draw->uTranslation, (vec3){draw->pos[0] / w, draw->pos[1] / h, 0});
    glm_rotate(draw->uRotation, draw->rotAngle, (vec3){0, 0, 1});
    glm_scale(draw->uScaling, (vec3){draw->rectSize[0] / w, draw->rectSize[1] / h, 0});

    // configure uniforms...
    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uTime");
    if (loc != -1)
    {
        glUniform1f(loc, glfwGetTime());
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uResolution");
    if (loc != -1)
    {
        glUniform2fv(loc, 1, draw->uResolution);
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uRectPos");
    if (loc != -1)
    {
        glUniform2fv(loc, 1, draw->pos);
    }

    loc = glGetUniformLocation(cgl_object_get_ID((struct cgl_object *)prog), "uRectSize");
    if (loc != -1)
    {
        glUniform2fv(loc, 1, draw->rectSize);
    }

    // uniform matrices
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
    cgl_vao_bind(&b->draw.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    cgl_vao_unbind();
}

void BallDelete(Ball *b)
{
    DrawableDelete(&b->draw);
}
