/*************************************************************************
	> File Name: resamplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"
#include "samplerate.h"
#include "iniparser.h"

#define N 1024

static char *NOTICE = "\n\
-------------------------------------------------------------------------------------------------\n\
                    AudioResamplerate  Copyright (C) 2016  AnSwErYWJ\n\
\n\
        This program comes with ABSOLUTELY NO WARRANTY;\n\
        This is free software, and you are welcome to redistribute it under certain conditions.\n\
-------------------------------------------------------------------------------------------------\n\
";

static int channels;
static double ratio;
static FILE *infp = NULL;
static FILE *outfp = NULL;
static short *tmpbuf = NULL;
static float *inbuf = NULL;
static float *outbuf = NULL;
static SRC_STATE *state;

static size_t count_in;
static size_t count_out;

static void parser(int *channels,double *ratio)
{
    dictionary *ini;

    /* parser dictionary */
    ini = iniparser_load("config.ini");
    if(ini == NULL)
    {
        LOGE("can not open %s","../config.ini");
        exit(EXIT_FAILURE);
    }

    /* get section counts and name */
    int n = iniparser_getnsec(ini);
    char *section = iniparser_getsecname(ini,0);

    if (n != 1 || strcmp(section,"params") != 0)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }

    /* print config */
    LOGI("resamplerate config:\n");
    iniparser_dump(ini,stderr);

    /* get channels */
    *channels = iniparser_getint(ini,"params:channels",-1);
    if (*channels <= 0)
    {
        LOGE("params:channels error!");
        exit(EXIT_FAILURE);
    }

    /* get ratio */
    double out_samrate = iniparser_getdouble(ini,"params:out_samrate",-1);
    double in_samrate = iniparser_getdouble(ini,"params:in_samrate",-1);
    if (in_samrate <= 0 || out_samrate <= 0)
    {
        LOGE("params:samrate error!");
        exit(EXIT_FAILURE);
    }
    *ratio = out_samrate/in_samrate;

    /* free dirctionary */
    iniparser_freedict(ini);
}

static void init(const char *in,const char *out)
{
    /* fopen in and out audio */
    infp = fopen(in,"rb");
    if (infp == NULL)
    {
        LOGE("open input file %s failed",in);
        exit(EXIT_FAILURE);
    }

    outfp = fopen(out, "wb+");
    if (outfp == NULL)
    {
        LOGE("create output file %s failed",out);
        exit(EXIT_FAILURE);
    }

    /* data items*/
    count_in = N;
    count_out = ((size_t)(ratio+1))*N;

    /* calloc memory */
    inbuf = (float *)calloc(1,count_in*sizeof(float));
    if (inbuf == NULL)
    {
        LOGE("inbuf calloc failed");
        exit(EXIT_FAILURE);
    }

    outbuf = (float *)calloc(1,count_out*sizeof(float));
    if (outbuf == NULL)
    {
        LOGE("outbuf calloc failed");
        exit(EXIT_FAILURE);
    }

    tmpbuf = (short *)calloc(1,count_out*sizeof(short));
    if (tmpbuf == NULL)
    {
        LOGE("tmpbuf calloc failed");
        exit(EXIT_FAILURE);
    }
}

static void resamplerate()
{
    SRC_DATA samplerate;
    int error;

    /* */
    if ((state = src_new (SRC_SINC_BEST_QUALITY, channels, &error)) == NULL)
    {
        LOGE("resample new failed");
        exit(EXIT_FAILURE);
    }

    /* samplerate params config */
    samplerate.data_in = inbuf;
    samplerate.input_frames = (long)count_in;
    samplerate.data_out = outbuf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = ratio;
    samplerate.output_frames = (long)count_out;

    size_t nread = 0;
    //while ((counts = read(infd, tmpbuf, N*sizeof(short))) > 0)
    while ((nread = fread(tmpbuf,sizeof(short),count_in,infp)) > 0)
    {
        src_short_to_float_array (tmpbuf, inbuf, nread) ;

        if ((error = src_process (state, &samplerate)))
            LOGE("src_process failed : %s",src_strerror (error)) ;

        memset(tmpbuf,'\0',count_out*sizeof(short));
        src_float_to_short_array (outbuf, tmpbuf, samplerate.output_frames_gen);

        if (fwrite(tmpbuf,sizeof(short),samplerate.output_frames_gen,outfp) != samplerate.output_frames_gen)
        {
            LOGE("fwrite failed");
            exit(EXIT_FAILURE);
        }

        memset(inbuf,'\0',count_in*sizeof(short));
        memset(outbuf,'\0',count_out*sizeof(short));
    }
}

static void delete()
{
    fclose(infp);
    fclose(outfp);

    free(inbuf);
    inbuf = NULL;
    free(outbuf);
    outbuf = NULL;
    free(tmpbuf);
    tmpbuf = NULL;

    src_delete(state);
}

int main(int argc,const char *argv[])
{
    LOGI("%s",NOTICE);

    if (argc != 3)
    {
        LOGE("Usage: ./resamplerate InputFile OutputFile");
        exit(EXIT_FAILURE);
    }

    /* parser config */
    parser(&channels,&ratio);

    init(argv[1],argv[2]);

    resamplerate();

    delete();

    return 0;
}
