/*************************************************************************
	> File Name: log.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Wed 24 Aug 2016 09:14:38 AM CST
 ************************************************************************/

#include <stdio.h>
#include "log.h"
#include <stdarg.h>

void print_log(const char level, const char *fmt, ...)
{
	va_list ap;
	char va_buf[VA_SIZE] = {'\0'};
	int counts;

	va_start(ap,fmt);

    /* format ap */
	counts = vsnprintf(va_buf,(VA_SIZE - 1),fmt,ap);
    if(counts < 0)
        fprintf(stderr," LOG ERROR !\n");

    va_end(ap);

    printf("[%c][%s] File:%s,Line:%d,Function:%s ## %s\n",level,__TIME__,__FILE__,__LINE__,__func__,va_buf);
}
