/*************************************************************************
	> File Name: resamplerate.h
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 25 Aug 2016 10:15:08 AM CST
 ************************************************************************/

#ifndef _RESAMPLERATE_H
#define _RESAMPLERATE_H

/* copyright */
#define NOTICE "\n\
-------------------------------------------------------------------------------------------------\n\
                    AudioResamplerate  Copyright (C) 2016-2017  AnSwErYWJ\n\
\n\
        This program comes with ABSOLUTELY NO WARRANTY;\n\
        This is free software, and you are welcome to redistribute it under certain conditions.\n\
-------------------------------------------------------------------------------------------------\n\
"

/* audio file struct*/
typedef struct _audio_file{
    char *path;
    char *type;
    FILE *fp;
    float *buf;
    size_t items;
    double sample_rate;
    int frame_len;
}AUDIO;

static int file_size(const char *filepath);
static AUDIO audio_new();
static void audio_del(AUDIO *file);
static void get_conf();
static void initialize();
static void resamplerate();
static void clean_up();

#endif
