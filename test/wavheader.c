/*************************************************************************
	> File Name: wavheader.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 03 Jan 2017 04:28:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/waveHeader/handle_wave.h"

#define filepath "../audio/ring.wav"

int main(int argc,char *argv[])
{
    WaveHeader_t w;
    init_wavheader(&w);
    print_wavheader(w);


	WaveHeader_t wavheader;
    FILE *fp = NULL;

    fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        fprintf(stderr,"open file: %s failed.\n",filepath);
        exit(EXIT_FAILURE);
    }
    
    /* read */
    if (read_wavheader(fp,&wavheader) == -1)
        fprintf(stderr,"read header failed.\n");
    
    fclose(fp);
    fp = NULL;

    fp = fopen("./tmp.wav", "wb+");
    if (fp == NULL)
    {
        fprintf(stderr,"open temp file failed.\n");
        exit(EXIT_FAILURE);
    }

    /* write */
    if (write_wavheader(fp,wavheader) == -1)
        fprintf(stderr,"write header failed.\n");
  
    fseek(fp,0,SEEK_SET);
    
    WaveHeader_t new_wavheader;
    
    /* read */
    if (read_wavheader(fp,&new_wavheader) == -1)
        fprintf(stderr,"read header failed.\n");

    fclose(fp);
    fp = NULL;

    remove("./tmp.wav");

    print_wavheader(new_wavheader);
    
    return 0;
}

