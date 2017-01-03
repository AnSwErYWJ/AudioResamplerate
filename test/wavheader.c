/*************************************************************************
	> File Name: wavheader.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 03 Jan 2017 04:28:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/waveHeader/handle_wave.h"

int main(int argc,char *argv[])
{
	WaveHeader_t wavheader;
	FILE *fp = NULL;

    fp = fopen("../audio/S16bit-LE-16kHz-Mono.wav", "rb");
    if (fp == NULL)
    {
        fprintf(stderr,"open file failed");
        exit(EXIT_FAILURE);
    }

    fread(wavheader.riff_id,4,1,fp);
    printf("wavheader.riff_id: %c%c%c%c\n",wavheader.riff_id[0],wavheader.riff_id[1],wavheader.riff_id[2],wavheader.riff_id[3]);

    wavheader.riff_datasize = get_int(fp);
    printf("wavheader.riff_datasize: %d\n",wavheader.riff_datasize);

    fread(wavheader.riff_type,4,1,fp);
    printf("wavheader.riff_type: %c%c%c%c\n",wavheader.riff_type[0],wavheader.riff_type[1],wavheader.riff_type[2],wavheader.riff_type[3]);

    fread(wavheader.fmt_id,4,1,fp);
    printf("wavheader.fmt_id: %c%c%c%c\n",wavheader.fmt_id[0],wavheader.fmt_id[1],wavheader.fmt_id[2],wavheader.fmt_id[3]);

    wavheader.fmt_datasize = get_int(fp);
    printf("wavheader.fmt_datasize: %d\n",wavheader.fmt_datasize);

    wavheader.fmt_compression_code = get_sint(fp);
    printf("wavheader.fmt_compression_code: %hd\n",wavheader.fmt_compression_code);
    
    wavheader.fmt_channels = get_sint(fp);
    printf("wavheader.fmt_channels: %hd\n",wavheader.fmt_channels);

    wavheader.fmt_sample_rate = get_int(fp);
    printf("wavheader.fmt_sample_rate: %d\n",wavheader.fmt_sample_rate);
    
    wavheader.fmt_avg_bytes_per_sec = get_int(fp);
    printf("wavheader.fmt_avg_bytes_per_sec: %d\n",wavheader.fmt_avg_bytes_per_sec);

    wavheader.fmt_block_align = get_sint(fp);
    printf("wavheader.fmt_block_align: %hd\n",wavheader.fmt_block_align);
    
    wavheader.fmt_bit_per_sample = get_sint(fp);
    printf("wavheader.fmt_bit_per_sample: %hd\n",wavheader.fmt_bit_per_sample);
    
    fread(wavheader.data_id,4,1,fp);
    printf("wavheader.data_id: %c%c%c%c\n",wavheader.data_id[0],wavheader.data_id[1],wavheader.data_id[2],wavheader.data_id[3]);
    
    wavheader.data_datasize = get_int(fp);
    printf("wavheader.data_datasize: %d\n",wavheader.data_datasize);
    
    fclose(fp);

    return 0;
}

