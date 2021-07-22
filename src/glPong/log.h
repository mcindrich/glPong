#ifndef glPong_LOG_H
#define glPong_LOG_H

#define LogError(fmt, ...) fprintf(stderr, "[err]>" fmt "\n", ##__VA_ARGS__)
#define LogInfo(fmt, ...) fprintf(stdout, "[inf]>" fmt "\n", ##__VA_ARGS__)
#define LogDebug(fmt, ...) fprintf(stdout, "[dbg]> " fmt "\n", ##__VA_ARGS__)

#endif // glPong_LOG_H