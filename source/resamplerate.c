/*************************************************************************
	> File Name: resamplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <fcntl.h>
#include <string.h>

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
//static int infd;
//static int outfd;
FILE *infp;
FILE *outfp;
static short *tmpbuf = NULL;
static float *inbuf = NULL;
static float *outbuf = NULL;
static size_t tmpSize;
static SRC_STATE *state;
static int error;

static void parser(int *channels,double *ratio)
{
    dictionary *ini;

    /* parser dictionary */
    ini = iniparser_load("config.ini");
    if(ini == NULL)
    {
        fprintf(stderr,"Error: can not open %s\n","../config.ini");
        exit(EXIT_FAILURE);
    }

    /* get section counts and name */
    int n = iniparser_getnsec(ini);
    char *section = iniparser_getsecname(ini,0);

    if (n != 1 || strcmp(section,"params") != 0)
    {
        fprintf(stderr,"Error: config error!\n");
        exit(EXIT_FAILURE);
    }

    /* print config */
    printf("resamplerate config:\n");
    iniparser_dump(ini,stderr);
    printf("\n");

    /* get channels */
    *channels = iniparser_getint(ini,"params:channels",-1);
    if (*channels == -1)
    {
        fprintf(stderr,"Error: params:channels error!\n");
        exit(EXIT_FAILURE);
    }

    /* get ratio */
    double out_samrate = iniparser_getdouble(ini,"params:out_samrate",-1);
    double in_samrate = iniparser_getdouble(ini,"params:in_samrate",-1);
    if (in_samrate == -1 || out_samrate == -1)
    {
        fprintf(stderr,"Error: params:samrate error!\n");
        exit(EXIT_FAILURE);
    }

    *ratio = out_samrate/in_samrate;

    /* free dirctionary */
    iniparser_freedict(ini);
}

static void init(const char *in,const char *out)
{

    /* open in and out audio */
    /*infd = open(in, O_RDONLY);
    if (infd == -1)
    {
        fprintf(stderr,"Error : open input file %s failed.\n",in);
        exit(EXIT_FAILURE);
    }

    outfd = open(out, O_WRONLY | O_CREAT, 0644);
    if (outfd == -1)
    {
        fprintf(stderr,"Error : create  output file %s failed.\n",out);
        exit(EXIT_FAILURE);
    }*/

    infp = fopen(in,"rb");
    if (infp == NULL)
    {
        fprintf(stderr,"Error : open input file %s failed.\n",in);
        exit(EXIT_FAILURE);
    }

    outfp = open(out, "wb+");
    if (outfp == NULL)
    {
        fprintf(stderr,"Error : create  output file %s failed.\n",out);
        perror("fopen out");
        exit(EXIT_FAILURE);
    }

    /* calloc memory */
    inbuf = (float *)calloc(1,N*sizeof(float));
    if (inbuf == NULL)
    {
        fprintf(stderr,"calloc failed\n");
        exit(EXIT_FAILURE);
    }

    outbuf = (float *)calloc(1,ratio*N*sizeof(float));
    if (outbuf == NULL)
    {
        fprintf(stderr,"calloc failed\n");
        exit(EXIT_FAILURE);
    }

    tmpSize = ratio >= 1 ? ratio*N*sizeof(short): N*sizeof(short);

    tmpbuf = (short *)calloc(1,tmpSize);
    if (tmpbuf == NULL)
    {
        fprintf(stderr,"calloc failed\n");
        exit(EXIT_FAILURE);
    }

}

static void resamplerate()
{
    SRC_DATA samplerate;
    //int counts;
    size_t counts;

    if ((state = src_new (SRC_SINC_BEST_QUALITY, channels, &error)) == NULL)
    {
        fprintf(stderr,"resample new failed");
        exit(EXIT_FAILURE);
    }

    /* samplerate config*/
    samplerate.data_in = inbuf;
    samplerate.input_frames = (long)(N);
    samplerate.data_out = outbuf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = ratio;
    samplerate.output_frames = (long)(samplerate.src_ratio * samplerate.input_frames);

    //while ((counts = read(infd, tmpbuf, N*sizeof(short))) > 0)
    while ((counts = fread(tmpbuf,tmpSize,1,infp)) > 0)
    {
        src_short_to_float_array (tmpbuf, inbuf, counts/sizeof(short)) ;

        if ((error = src_process (state, &samplerate)))
            printf ("src_process failed : %s\n",src_strerror (error)) ;

        memset(tmpbuf,'\0',tmpSize);
        src_float_to_short_array (outbuf, tmpbuf, samplerate.output_frames_gen);

        //if (write(outfd,tmpbuf,samplerate.output_frames_gen*sizeof(short)) != samplerate.output_frames_gen*sizeof(short))
        if ((fwrite(tmpbuf,sizeof(short),samplerate.output_frames_gen,outfp)) != (samplerate.output_frames_gen*sizeof(short)))
        {
            printf("文件写失败！\n");
            exit(EXIT_FAILURE);
        }

        memset(inbuf,'\0',N*sizeof(float));
        memset(outbuf,'\0',ratio*N*sizeof(float));
    }
}

static void delete()
{
    //close(infd);
    //close(outfd);

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
    printf("%s\n",NOTICE);

    if (argc != 3)
    {
        fprintf(stderr,"Usage: ./resamplerate InputFile OutputFile\n");
        exit(EXIT_FAILURE);
    }

    /* parser config */
    parser(&channels,&ratio);

    init(argv[1],argv[2]);

    resamplerate();

    delete();

    return 0;
}
