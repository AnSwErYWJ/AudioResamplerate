/*************************************************************************
	> File Name: resamplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "samplerate.h"
#include "iniparser.h"
#include "log.h"

#define N 1024 //items

static char *NOTICE = "\n\
-------------------------------------------------------------------------------------------------\n\
                    AudioResamplerate  Copyright (C) 2016  AnSwErYWJ\n\
\n\
        This program comes with ABSOLUTELY NO WARRANTY;\n\
        This is free software, and you are welcome to redistribute it under certain conditions.\n\
-------------------------------------------------------------------------------------------------\n\
";

/* ini params */
static int channels;
static double ratio;
static char *input;
static char *output;

static FILE *infp = NULL;
static FILE *outfp = NULL;
static short *tmpbuf = NULL;
static float *inbuf = NULL;
static float *outbuf = NULL;

static size_t count_in;
static size_t count_out;

static dictionary *ini;
static SRC_STATE *state;

static int error;

static void get_conf()
{
    /* parser dictionary */
    ini = iniparser_load("config.ini");
    if(ini == NULL)
    {
        LOGE("can not open %s","../config.ini");
        exit(EXIT_FAILURE);
    }

    /* get section numbers */
    int n = iniparser_getnsec(ini);
    if (n != 2)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }

    /* get section names */
    char *section_one = iniparser_getsecname(ini,0);
    if (strcmp(section_one,"audio") != 0)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }
    char *section_two = iniparser_getsecname(ini,1);
    if (strcmp(section_two,"other") != 0)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }

    /* get channels */
    channels = iniparser_getint(ini,"audio:channels",-1);
    if (channels <= 0)
    {
        LOGE("audio:channels error!");
        exit(EXIT_FAILURE);
    }

    /* get ratio */
    double out_samrate = iniparser_getdouble(ini,"audio:out_samrate",-1);
    double in_samrate = iniparser_getdouble(ini,"audio:in_samrate",-1);
    if (in_samrate <= 0 || out_samrate <= 0)
    {
        LOGE("audio:samrate error!");
        exit(EXIT_FAILURE);
    }
    ratio = out_samrate/in_samrate;

    /* get input_file */
    input = iniparser_getstring(ini,"other:input_file","null");
    if (strcmp(input,"null") == 0 )
    {
        LOGE("other:input_file!");
        exit(EXIT_FAILURE);
    }

    /* get output_file */
    output = iniparser_getstring(ini,"other:output_file","null");
    if (strcmp(output,"null") == 0 )
    {
        LOGE("other:output_file!");
        exit(EXIT_FAILURE);
    }
}

static void initialize()
{
    /* open in and out audio */
    infp = fopen(input,"rb");
    if (infp == NULL)
    {
        LOGE("open input file %s failed",input);
        exit(EXIT_FAILURE);
    }

    outfp = fopen(output, "wb+");
    if (outfp == NULL)
    {
        LOGE("create output file %s failed",output);
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

    /* new samplerate obj*/
    if ((state = src_new (SRC_SINC_BEST_QUALITY, channels, &error)) == NULL)
    {
        LOGE("resample new failed");
        exit(EXIT_FAILURE);
    }
}

static void resamplerate()
{
    SRC_DATA samplerate;

    /* samplerate params config */
    samplerate.data_in = inbuf;
    samplerate.input_frames = (long)count_in;
    samplerate.data_out = outbuf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = ratio;
    samplerate.output_frames = (long)count_out;

    size_t nread = 0;

    /* resamplerate */
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

static void clean_up()
{
    /* close audio file */
    fclose(infp);
    fclose(outfp);

    /* free memory */
    free(inbuf);
    inbuf = NULL;
    free(outbuf);
    outbuf = NULL;
    free(tmpbuf);
    tmpbuf = NULL;

    /* free samplerate obj*/
    src_delete(state);

    /* free dirctionary */
    iniparser_freedict(ini);
}

int main(int argc,const char *argv[])
{
    printf("%s",NOTICE);

    get_conf();

    initialize();

    resamplerate();

    printf("output path : %s\n",output);
    printf("successfully\n");

    clean_up();

    return 0;
}
