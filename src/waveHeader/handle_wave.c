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
int get_int(FILE *fp)
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

int put_uint(int i,FILE *fp)
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

short int get_sint(FILE *fp)
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

short int put_sint(short int i,FILE *fp)
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
