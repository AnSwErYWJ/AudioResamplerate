/*************************************************************************
	> File Name: test-samplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include "samplerate.h"

#define CHANNELS 1
#define RATIO 48000/16000
#define N 1024

#define NOTICE "\
-------------------------------------------------------------------------------------------------\n\
                    AudioResamplerate  Copyright (C) 2016  AnSwErYWJ\n\
\n\
        This program comes with ABSOLUTELY NO WARRANTY;\n\
        This is free software, and you are welcome to redistribute it under certain conditions.\n\
-------------------------------------------------------------------------------------------------\n\
"

int main(int argc,const char *argv[])
{
    SRC_DATA samplerate;
    SRC_STATE *state;
    
    int error;
    int infd;
    int outfd;
    int counts;

    printf("%s\n",NOTICE);

    if (argc != 3)
    {
        fprintf(stderr,"Usage: ./resamplerate InputFile OutputFile\n");
        exit(EXIT_FAILURE);
    }

    infd = open(argv[1], O_RDONLY);
    if (infd == -1)
    {
        fprintf(stderr,"Error : open input file %s failed.\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    
    outfd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (outfd == -1)
    {
        fprintf(stderr,"Error : create  output file %s failed.\n",argv[2]);
        exit(EXIT_FAILURE);
    }

    short *tmpbuf = (short *)calloc(1,RATIO*N*sizeof(short));
    if (tmpbuf == NULL)
        printf("calloc failed\n");

    float *inbuf = (float *)calloc(1,N*sizeof(float));
    if (inbuf == NULL)
        printf("calloc failed\n");

    float *outbuf = (float *)calloc(1,RATIO*N*sizeof(float));
    if (outbuf == NULL)
        printf("calloc failed\n");

    if ((state = src_new (SRC_SINC_BEST_QUALITY, CHANNELS, &error)) == NULL)
            fprintf(stderr,"resample new failed");

    samplerate.data_in = inbuf;
    samplerate.input_frames = (long)(N);
    samplerate.data_out = outbuf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = (double)RATIO;
    samplerate.output_frames = (long)(samplerate.src_ratio * samplerate.input_frames);

    while ((counts = read(infd, tmpbuf, N*sizeof(short))) > 0)
    {
        src_short_to_float_array (tmpbuf, inbuf, counts/sizeof(short)) ;

        if ((error = src_process (state, &samplerate)))
            printf ("src_process failed : %s\n",src_strerror (error)) ;

        memset(tmpbuf,'\0',RATIO*N*sizeof(short));
        src_float_to_short_array (outbuf, tmpbuf, samplerate.output_frames_gen);

        if (write(outfd,tmpbuf,samplerate.output_frames_gen*sizeof(short)) != samplerate.output_frames_gen*sizeof(short))
        {
            printf("文件写失败！\n");
            return 1;
        }

        memset(inbuf,'\0',N*sizeof(float));
        memset(outbuf,'\0',RATIO*N*sizeof(float));
    }

    close(infd);
    close(outfd);

    free(inbuf);
    inbuf = NULL;
    free(outbuf);
    outbuf = NULL;
    free(tmpbuf);
    tmpbuf = NULL;

    src_delete(state);

    return 0;
}
