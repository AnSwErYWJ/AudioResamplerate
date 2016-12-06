/*************************************************************************
	> File Name: handle_wave.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 06 Dec 2016 04:10:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define sin_t       5
#define sin_hz      1000
#define sin_db 100
#define size_buf    sin_t*100000

unsigned char buf[size_buf*2];




//WaveHeader WavInf = {
//    "RIFF",
//    sin_t*500*2+36,
//    "WAVE",
//    "fmt ",
//    16,
//    1,
//    2,
//    100000,
//    100000,
//    1,
//    8,
//    "data",
//    500*sin_t*2
//};

int get_int(FILE *fp)
{
    char *s;
    int i;
    s = (char *)&i;
    s[0]=getc(fp);
    printf("%x\n",s[0]);
    s[1]=getc(fp);
    printf("%x\n",s[1]);
    s[2]=getc(fp);
    printf("%x\n",s[2]);
    s[3]=getc(fp);
    printf("%x\n",s[3]);
    return i;
}


int main(int argc,char *argv[])
{
	WaveHeader wavinfo;
	FILE *fp = NULL;
    //printf("%ld\n",sizeof(wavinfo));
	//char *buf = (char *)malloc(4);

    //FILE *fp = fopen("./test.wav", "rb");
    //if (fp == NULL)
    //{
    //    fprintf(stderr,"open file failed");
    //    exit(EXIT_FAILURE);
    //}
    fp = fopen("./log", "wb");
    if (fp == NULL)
    {
        fprintf(stderr,"open file failed");
        exit(EXIT_FAILURE);
    }

    putw(0xee,fp);
    fclose(fp);
    fp = fopen("./log", "rb");
    if (fp == NULL)
    {
        fprintf(stderr,"open file failed");
        exit(EXIT_FAILURE);
    }

    //fread(wavinfo.chRIFF,4,1,fp);
    //printf("%c %c %c %c\n",wavinfo.chRIFF[0],wavinfo.chRIFF[1],wavinfo.chRIFF[2],wavinfo.chRIFF[3]);

    wavinfo.total_Len = get_int(fp);
    printf("%x\n",wavinfo.total_Len);

    //fread(buf,4,1,fp);
    //printf("%d %d %d %d\n",buf[0],buf[1],buf[2],buf[3]);

    fclose(fp);

    return 0;
}


