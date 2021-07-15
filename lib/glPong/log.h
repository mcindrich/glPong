#ifndef glPong_LOG_H
#define glPong_LOG_H

#define log_err(fmt, ...) fprintf(stderr, "[err]>" fmt "\n", ##__VA_ARGS__)
#define log_inf(fmt, ...) fprintf(stdout, "[inf]>" fmt "\n", ##__VA_ARGS__)
#define log_dbg(fmt, ...) fprintf(stdout, "[dbg]> " fmt "\n", ##__VA_ARGS__)

#endif // glPong_LOG_H