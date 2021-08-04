#include <glPong/game/config.h>
#include <cJSON.h>

static void configLoadDefaults(GameConfig *cfg);
static char *readFile(const char *path);
static int configExists(const char *path);

void GameConfigInit(GameConfig *cfg)
{
    glm_vec2_copy((vec2){0, 0}, cfg->ballData.rectSize);
}

int GameConfigLoad(GameConfig *cfg, const char *path)
{
    int err = 0;
    char *cfgData = NULL;
    FILE *fptr = NULL;
    // initially load defaults, after that read config if one exists
    configLoadDefaults(cfg);

    if (configExists(path))
    {
        // load config and read the game configuration
        cfgData = readFile(path);
        if (cfgData)
        {
            cJSON *root = cJSON_Parse(cfgData);
            cJSON *paddle = NULL;
            cJSON *ball = NULL;

            paddle = cJSON_GetObjectItem(root, "paddle");
            ball = cJSON_GetObjectItem(root, "ball");
            if (paddle)
            {
                cJSON *paddleW = cJSON_GetObjectItem(paddle, "width");
                cJSON *paddleH = cJSON_GetObjectItem(paddle, "height");
                cJSON *paddleSpeed = cJSON_GetObjectItem(paddle, "speed");

                if (cJSON_IsNumber(paddleW) && cJSON_IsNumber(paddleH))
                {
                    glm_vec2_copy((vec2){paddleW->valuedouble, paddleH->valuedouble}, cfg->paddleData.rectSize);
                }

                if (cJSON_IsNumber(paddleSpeed))
                {
                    cfg->paddleData.speed = paddleSpeed->valuedouble;
                }
            }
            if (ball)
            {
                cJSON *ballW = cJSON_GetObjectItem(ball, "width");
                cJSON *ballH = cJSON_GetObjectItem(ball, "height");
                cJSON *ballSpeed = cJSON_GetObjectItem(ball, "speed");

                if (cJSON_IsNumber(ballW) && cJSON_IsNumber(ballH))
                {
                    glm_vec2_copy((vec2){ballW->valuedouble, ballH->valuedouble}, cfg->ballData.rectSize);
                }

                if (cJSON_IsNumber(ballSpeed))
                {
                    cfg->ballData.speed = ballSpeed->valuedouble;
                }
            }
            cJSON_Delete(root);
            free(cfgData);
        }
        else
        {
            err = -1;
        }
    }
    else
    {
        // config doesn't exist -> form a config from default values and save to file
        cJSON *root = cJSON_CreateObject();
        cJSON *paddle = cJSON_CreateObject();
        cJSON *ball = cJSON_CreateObject();

        cJSON *ballSpeed = cJSON_CreateNumber(cfg->ballData.speed);
        cJSON *paddleSpeed = cJSON_CreateNumber(cfg->paddleData.speed);

        cJSON *ballRectW = cJSON_CreateNumber(cfg->ballData.rectSize[0]);
        cJSON *ballRectH = cJSON_CreateNumber(cfg->ballData.rectSize[1]);
        cJSON *paddleRectW = cJSON_CreateNumber(cfg->paddleData.rectSize[0]);
        cJSON *paddleRectH = cJSON_CreateNumber(cfg->paddleData.rectSize[1]);

        // speed
        cJSON_AddItemToObject(ball, "speed", ballSpeed);
        cJSON_AddItemToObject(paddle, "speed", paddleSpeed);

        // W x H
        cJSON_AddItemToObject(ball, "width", ballRectW);
        cJSON_AddItemToObject(ball, "height", ballRectH);

        cJSON_AddItemToObject(paddle, "width", paddleRectW);
        cJSON_AddItemToObject(paddle, "height", paddleRectH);

        cJSON_AddItemToObject(root, "paddle", paddle);
        cJSON_AddItemToObject(root, "ball", ball);

        fptr = fopen(path, "w");
        if (!fptr)
        {
            cJSON_Delete(root);
            return -1;
        }

        char *str_val = cJSON_Print(root);

        // file open for writing
        fprintf(fptr, "%s", str_val);
        fclose(fptr);
        free(str_val);
        cJSON_Delete(root);
    }
    return err;
}

int GameConfigExport(GameConfig *cfg, const char *path)
{
    int err = 0;
    return err;
}

void GameConfigDelete(GameConfig *cfg)
{
}

static void configLoadDefaults(GameConfig *cfg)
{
    cfg->paddleData.speed = 0.03;
    cfg->ballData.speed = 0.01;
    glm_vec2_copy((vec2){20, 70}, cfg->paddleData.rectSize);
    glm_vec2_copy((vec2){20, 20}, cfg->ballData.rectSize);
}

static int configExists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

static char *readFile(const char *path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(sizeof(char) * (length + 1));
        if (buffer)
        {
            fread(buffer, sizeof(char), length, f);
        }
        buffer[length] = 0;
        fclose(f);
    }

    return buffer;
}