/*************************************************************************
	> File Name: resamplerate.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Fri Aug  5 16:25:02 2016
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "samplerate.h"
#include "iniparser.h"
#include "log.h"
#include "resamplerate.h"
#include "handle_wave.h"

static AUDIO input,output;
static dictionary *ini;
static SRC_STATE *state;

static int channels = 0;
static double ratio = 0;

static int tmp_frame_len = 0;
static short *tmpbuf = NULL;
static int error;

static int file_size(const char *filepath)
{
    struct stat statbuf;

    stat(filepath,&statbuf);

    int size = statbuf.st_size;

    return size;
}

/* new AUDIO obj */
static AUDIO audio_new()
{
    AUDIO file;

    file.path = NULL;
    file.type = NULL;
    file.fp = NULL;
    file.buf = NULL;
    file.items = 0;
    file.sample_rate = 0;
    file.frame_len = 0;

    return file;
}

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
    if (n != 3)
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
    if (strcmp(section_two,"path") != 0)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }
    char *section_three = iniparser_getsecname(ini,2);
    if (strcmp(section_three,"type") != 0)
    {
        LOGE("config error!");
        exit(EXIT_FAILURE);
    }

    /* get channels */
    channels = iniparser_getint(ini,"audio:channels",-1);
    if (channels <= 0 || channels > 2)
    {
        LOGE("audio:channels error,only support mono or stereo!");
        exit(EXIT_FAILURE);
    }

    /* get ratio */
    output.sample_rate = iniparser_getdouble(ini,"audio:output_sample_rate",-1);
    input.sample_rate = iniparser_getdouble(ini,"audio:input_sample_rate",-1);
    if (input.sample_rate <= 0 || output.sample_rate <= 0)
    {
        LOGE("audio:sample_rate error!");
        exit(EXIT_FAILURE);
    }
    ratio = output.sample_rate/input.sample_rate;

    /* get file path */
    input.path = iniparser_getstring(ini,"path:input","null");
    if (strcmp(input.path,"null") == 0 )
    {
        LOGE("path:input!");
        exit(EXIT_FAILURE);
    }

    output.path = iniparser_getstring(ini,"path:output","null");
    if (strcmp(output.path,"null") == 0 )
    {
        LOGE("path:output!");
        exit(EXIT_FAILURE);
    }

    /* get file type */
    input.type = iniparser_getstring(ini,"type:input","null");
    if (strcmp(input.type,"null") == 0 )
    {
        LOGE("type:input!");
        exit(EXIT_FAILURE);
    }

    output.type = iniparser_getstring(ini,"type:output","null");
    if (strcmp(output.type,"null") == 0 )
    {
        LOGE("type:output!");
        exit(EXIT_FAILURE);
    }
}

static void audio_del(AUDIO *file)
{
    file->path = NULL;
    file->type = NULL;

    fclose(file->fp);

    free(file->buf);
    file->buf = NULL;

    file->items = 0;
    file->sample_rate = 0;
    file->frame_len = 0;
}

static void initialize()
{

    input = audio_new();
    output = audio_new();

    get_conf();

    /* open in and out audio */
    input.fp = fopen(input.path,"rb");
    if (input.fp == NULL)
    {
        LOGE("open input file %s failed",input.path);
        exit(EXIT_FAILURE);
    }

    output.fp = fopen(output.path, "wb+");
    if (output.fp == NULL)
    {
        LOGE("create output file %s failed",output.path);
        exit(EXIT_FAILURE);
    }

    /* data items*/
    input.items = 1024;
    output.items = ((size_t)(ratio+1)) * input.items;

    int size;
    /* judge input file type */
    if (strcmp(input.type,"pcm") == 0 || strcmp(input.type,"PCM") == 0 )
    	size = file_size(input.path) * ratio;
    else if (strcmp(input.type,"wav") == 0 || strcmp(input.type,"WAV") == 0 )
    {
    	size = (file_size(input.path)-44) * ratio;
    	fseek(input.fp,44,SEEK_SET);
    }
    else
    {
    	LOGE("input file type %s invalid",input.type);
        exit(EXIT_FAILURE);
    }

    /* judge output file type */
    if (strcmp(output.type,"pcm") == 0 || strcmp(output.type,"PCM") == 0 ){}
    else if (strcmp(output.type,"wav") == 0 || strcmp(output.type,"WAV") == 0 )
    {
    	WaveHeader_t wavheader;
    	init_wavheader(&wavheader);

        wavheader.riff_datasize = size + 44 - 8;
        wavheader.fmt_channels = channels;
        wavheader.fmt_sample_rate = output.sample_rate;
        wavheader.fmt_avg_bytes_per_sec = wavheader.fmt_sample_rate * wavheader.fmt_channels * wavheader.fmt_bit_per_sample / 8;
        wavheader.fmt_block_align = wavheader.fmt_bit_per_sample * wavheader.fmt_channels / 8;

        wavheader.data_datasize = size;

        write_wavheader(output.fp,wavheader);
    }
    else
    {
    	LOGE("output file type %s invalid",input.type);
        exit(EXIT_FAILURE);
    }

    input.frame_len = sizeof(float)*channels;
    output.frame_len = sizeof(float)*channels;
    tmp_frame_len = sizeof(short)*channels;
    /* calloc memory */
    input.buf = (float *)calloc(1,input.items*input.frame_len);
    if (input.buf == NULL)
    {
        LOGE("input buf calloc failed");
        exit(EXIT_FAILURE);
    }

    output.buf = (float *)calloc(1,output.items*output.frame_len);
    if (output.buf == NULL)
    {
        LOGE("output buf calloc failed");
        exit(EXIT_FAILURE);
    }

    tmpbuf = (short *)calloc(1,output.items*tmp_frame_len);
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
    samplerate.data_in = input.buf;
    samplerate.input_frames = (long)input.items;
    samplerate.data_out = output.buf;
    samplerate.end_of_input = 0;
    samplerate.src_ratio = ratio;
    samplerate.output_frames = (long)output.items;

    size_t nread = 0;

    /* resamplerate */
    while ((nread = fread(tmpbuf,tmp_frame_len,input.items,input.fp)) > 0)
    {
        src_short_to_float_array (tmpbuf, input.buf, nread*channels) ;

        if ((error = src_process (state, &samplerate)))
            LOGE("src_process failed : %s",src_strerror (error)) ;

        memset(tmpbuf,'\0',output.items*tmp_frame_len);
        src_float_to_short_array (output.buf, tmpbuf, samplerate.output_frames_gen*channels);

        if (fwrite(tmpbuf,tmp_frame_len,samplerate.output_frames_gen,output.fp) != samplerate.output_frames_gen)
        {
            LOGE("fwrite failed");
            exit(EXIT_FAILURE);
        }

        memset(input.buf,'\0',input.items*input.frame_len);
        memset(output.buf,'\0',output.items*output.frame_len);
    }

}

static void clean_up()
{
    /* free AUDIO obj */
    audio_del(&input);
    audio_del(&output);

    /* free tmp buf */
    free(tmpbuf);
    tmpbuf = NULL;

    /* free samplerate obj*/
    src_delete(state);

    /* free dirctionary */
    iniparser_freedict(ini);
}

int main(int argc,const char *argv[])
{
	/* copyright */
    printf("%s",NOTICE);

    initialize();

    resamplerate();

    printf("output path : %s\n",output.path);
    printf("successfully\n");

    clean_up();

    return 0;
}
