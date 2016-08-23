/*************************************************************************
	> File Name: log.h
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Tue 23 Aug 2016 11:39:23 AM CST
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#define	LEVEL_VERBOSE 0
#define	LEVEL_DEBUG   1  //develop
#define	LEVEL_INFO    2  //release
#define	LEVEL_WARN    3
#define	LEVEL_ERROR   4

#if defined(LEVEL_VERBOSE)
#define LOG_VERBOSE(fmt,...)\
        do{\
            printf("[V][%s] File:%s,Line:%d,Function:%s ## "fmt"\n",__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);\
        }while(0)
#endif

#if defined(LEVEL_DEBUG)
#define LOG_DEBUG(fmt,...)\
        do{\
            printf("[D][%s] File:%s,Line:%d,Function:%s ## "fmt"\n",__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);\
        }while(0)
#endif

#if defined(LEVEL_INFO)
#define LOG_INFO(fmt,...)\
        do{\
            printf("[I][%s] File:%s,Line:%d,Function:%s ## "fmt"\n",__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);\
        }while(0)
#endif

#if defined(LEVEL_WARN)
#define LOG_WRAN(fmt,...)\
        do{\
            printf("[W][%s] File:%s,Line:%d,Function:%s ## "fmt"\n",__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);\
        }while(0)
#endif

#if defined(LEVEL_ERROR)
#define LOG_ERROR(fmt,...)\
        do{\
            printf("[E][%s] File:%s,Line:%d,Function:%s ## "fmt"\n",__TIME__,__FILE__,__LINE__,__func__,##__VA_ARGS__);\
        }while(0)
#endif

/* set log level */
#define LEVEL_DEFAULT LEVEL_DEBUG

#if LEVEL_DEFAULT == LEVEL_VERBOSE
    #define LOGV(fmt,...) LOG_VERBOSE(fmt,__VA_ARGS__)
    #define LOGD(fmt,...) LOG_DEBUG(fmt,__VA_ARGS__)
    #define LOGI(fmt,...) LOG_INFO(fmt,__VA_ARGS__)
    #define LOGW(fmt,...) LOG_WRAN(fmt,__VA_ARGS__)
    #define LOGE(fmt,...) LOG_ERROR(fmt,__VA_ARGS__)
#elif LEVEL_DEFAULT == LEVEL_DEBUG
    #define LOGV(fmt,...) do{}while(0)
    #define LOGD(fmt,...) LOG_DEBUG(fmt,__VA_ARGS__)
    #define LOGI(fmt,...) LOG_INFO(fmt,__VA_ARGS__)
    #define LOGW(fmt,...) LOG_WRAN(fmt,__VA_ARGS__)
    #define LOGE(fmt,...) LOG_ERROR(fmt,__VA_ARGS__)
#elif LEVEL_DEFAULT == LEVEL_INFO
    #define LOGV(fmt,...) do{}while(0)
    #define LOGD(fmt,...) do{}while(0)
    #define LOGI(fmt,...) LOG_INFO(fmt,__VA_ARGS__)
    #define LOGW(fmt,...) LOG_WRAN(fmt,__VA_ARGS__)
    #define LOGE(fmt,...) LOG_ERROR(fmt,__VA_ARGS__)
#elif LEVEL_DEFAULT == LEVEL_WARN
    #define LOGV(fmt,...) do{}while(0)
    #define LOGD(fmt,...) do{}while(0)
    #define LOGI(fmt,...) do{}while(0)
    #define LOGW(fmt,...) LOG_WRAN(fmt,__VA_ARGS__)
    #define LOGE(fmt,...) LOG_ERROR(fmt,__VA_ARGS__)
#elif LEVEL_DEFAULT == LEVEL_ERROR
    #define LOGV(fmt,...) do{}while(0)
    #define LOGD(fmt,...) do{}while(0)
    #define LOGI(fmt,...) do{}while(0)
    #define LOGW(fmt,...) do{}while(0)
    #define LOGE(fmt,...) LOG_ERROR(fmt,__VA_ARGS__)
#else
    #define LOGV(fmt,...) do{}while(0)
    #define LOGD(fmt,...) do{}while(0)
    #define LOGI(fmt,...) do{}while(0)
    #define LOGW(fmt,...) do{}while(0)
    #define LOGE(fmt,...) do{}while(0)
#endif

#endif