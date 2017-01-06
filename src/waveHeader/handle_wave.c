/*************************************************************************
	> File Name: handle_wave.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 06 Dec 2016 04:10:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "handle_wave.h"

/* read and write integer from file stream */
static int get_int(FILE *fp)
{
    char *s;
    int i;
    s = (char *)&i;
    size_t len = sizeof(int);
    int n = 0;
    for(;n < len;n++)
    {
    	s[n]=getc(fp);
    	//printf("%x\n",s[n]);
    }
    return i;
}

static int put_int(int i,FILE *fp)
{
    char *s;
    s=(char *)&i;
    size_t len = sizeof(int);
    int n = 0;
    for(;n < len;n++)
    {
    	putc(s[n],fp);
        //printf("%x\n",s[n]);
    }

    return i;
}

static short int get_sint(FILE *fp)
{
    char *s;
    short int i;
    s = (char *)&i;
    size_t len = sizeof(short);
    int n = 0;
    for(;n < len;n++)
    {
    	s[n]=getc(fp);
    	//printf("%x\n",s[n]);
    }

    return i;
}

static short int put_sint(short int i,FILE *fp)
{
    char *s;
    s=(char *)&i;
    size_t len = sizeof(short);
    int n = 0;
    for(;n < len;n++)
    {
    	putc(s[n],fp);
        //printf("%x\n",s[n]);
    };

    return i;
}

void init_wavheader(WaveHeader_t *wavheader)
{
	sprintf(wavheader->riff_id,"RIFF");
    wavheader->riff_datasize = -1;

    sprintf(wavheader->riff_type,"WAVE");

    sprintf(wavheader->fmt_id,"fmt ");
    wavheader->fmt_datasize = 16;
    wavheader->fmt_compression_code = 1;
    wavheader->fmt_channels = -1;
    wavheader->fmt_sample_rate = -1;
    wavheader->fmt_avg_bytes_per_sec = -1;
    wavheader->fmt_block_align = -1;
    wavheader->fmt_bit_per_sample = 16;

    sprintf(wavheader->data_id,"data");
    wavheader->data_datasize = -1;
}

int read_wavheader(FILE *fp,WaveHeader_t *wavheader)
{
	if (fp ==NULL)
		return -1;

    fread(wavheader->riff_id,4,1,fp);
    wavheader->riff_datasize = get_int(fp);
    fread(wavheader->riff_type,4,1,fp);
    fread(wavheader->fmt_id,4,1,fp);
    wavheader->fmt_datasize = get_int(fp);
    wavheader->fmt_compression_code = get_sint(fp);
    wavheader->fmt_channels = get_sint(fp);
    wavheader->fmt_sample_rate = get_int(fp);
    wavheader->fmt_avg_bytes_per_sec = get_int(fp);
    wavheader->fmt_block_align = get_sint(fp);
    wavheader->fmt_bit_per_sample = get_sint(fp);
    fread(wavheader->data_id,4,1,fp);
    wavheader->data_datasize = get_int(fp);

    return 0;
}

int write_wavheader(FILE *fp,WaveHeader_t wavheader)
{
	if (fp ==NULL)
		return -1;

    fwrite(wavheader.riff_id,4,1,fp);
    put_int(wavheader.riff_datasize,fp);
    fwrite(wavheader.riff_type,4,1,fp);
    fwrite(wavheader.fmt_id,4,1,fp);
    put_int(wavheader.fmt_datasize,fp);
    put_sint(wavheader.fmt_compression_code,fp);
    put_sint(wavheader.fmt_channels,fp);
    put_int(wavheader.fmt_sample_rate,fp);
    put_int(wavheader.fmt_avg_bytes_per_sec,fp);
    put_sint(wavheader.fmt_block_align,fp);
    put_sint(wavheader.fmt_bit_per_sample,fp);
    fwrite(wavheader.data_id,4,1,fp);
    put_int(wavheader.data_datasize,fp);

    return 0;
}

void print_wavheader(WaveHeader_t wavheader)
{
    printf("wavheader.riff_id: %c%c%c%c\n",wavheader.riff_id[0],wavheader.riff_id[1],wavheader.riff_id[2],wavheader.riff_id[3]);
    printf("wavheader.riff_datasize: %d\n",wavheader.riff_datasize);
    printf("wavheader.riff_type: %c%c%c%c\n",wavheader.riff_type[0],wavheader.riff_type[1],wavheader.riff_type[2],wavheader.riff_type[3]);
    printf("wavheader.fmt_id: %c%c%c%c\n",wavheader.fmt_id[0],wavheader.fmt_id[1],wavheader.fmt_id[2],wavheader.fmt_id[3]);
    printf("wavheader.fmt_datasize: %d\n",wavheader.fmt_datasize);
    printf("wavheader.fmt_compression_code: %hd\n",wavheader.fmt_compression_code);
    printf("wavheader.fmt_channels: %hd\n",wavheader.fmt_channels);
    printf("wavheader.fmt_sample_rate: %d\n",wavheader.fmt_sample_rate);
    printf("wavheader.fmt_avg_bytes_per_sec: %d\n",wavheader.fmt_avg_bytes_per_sec);
    printf("wavheader.fmt_block_align: %hd\n",wavheader.fmt_block_align);
    printf("wavheader.fmt_bit_per_sample: %hd\n",wavheader.fmt_bit_per_sample);
    printf("wavheader.data_id: %c%c%c%c\n",wavheader.data_id[0],wavheader.data_id[1],wavheader.data_id[2],wavheader.data_id[3]);
    printf("wavheader.data_datasize: %d\n",wavheader.data_datasize);
}

