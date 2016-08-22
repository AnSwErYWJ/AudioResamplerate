#ifndef LOG_H
#define LOG_H

#define PROJECT "AudioResamplerate"

#define	LOG_LEVEL_VERBOSE 0
#define	LOG_LEVEL_DEBUG   1
#define	LOG_LEVEL_INFO    2
#define	LOG_LEVEL_WARN    3
#define	LOG_LEVEL_ERROR   4

#define LOG_LEVEL_DEFAULT LOG_LEVEL_VERBOSE

#define DEBUG_PRINT do{}while(0)

#if defined(DEBUG_PRINT)
#define DEBUG(...)\
        do{\
            printf("%s %s\n",__TIME__,__DATE__);\
            printf("[%s] %s:%d:%s():",PROJECT,__FILE__,__LINE__,__func__);\
            printf(__VA_ARGS__);\
        }while(0)
#endif

void print_msg(const char *file, int line, const char *func, const char *fmt, ...);

#if LOG_LEVEL_DEFAULT > LOG_LEVEL_WARN
#define LOGV(f, ...)
#define LOGD(f, ...)
#define LOGI(f, ...)
#define LOGW(f, ...)
#define LOGE(f, ...)
#elif LOG_LEVEL_DEFAULT > LOG_LEVEL_INFO
#define LOGV(f, ...)
#define LOGD(f, ...)
#define LOGI(f, ...)
#define LOGW(f, ...)
#define LOGE(f, ...)
#elif LOG_LEVEL_DEFAULT > LOG_LEVEL_DEBUG
#define LOGV(f, ...)
#define LOGD(f, ...)
#define LOGI(f, ...)
#define LOGW(f, ...)
#define LOGE(f, ...)
#elif LOG_LEVEL_DEFAULT > LOG_LEVEL_VERBOSE
#define LOGV(f, ...)
#define LOGD(f, ...)
#define LOGI(f, ...)
#define LOGW(f, ...)
#define LOGE(f, ...)
#else
#define LOGI(...) print_msg(__FILE__, __LINE__, __func__, " "__VA_ARGS__)
//#define LOGV(f, ...) printf("[PLAYER -V] File:%s, Line:%d, Function:%s "f"\n", __FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
//#define LOGD(f, ...) printf("[PLAYER -D] File:%s, Line:%d, Function:%s "f"\n", __FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
//#define LOGI(f, ...) printf("[PLAYER -I] File:%s, Line:%d, Function:%s "f"\n", __FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
//#define LOGW(f, ...) printf("[PLAYER -W] File:%s, Line:%d, Function:%s "f"\n", __FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
//#define LOGE(f, ...) printf("[PLAYER -E] File:%s, Line:%d, Function:%s "f"\n", __FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
#endif

#endif
