/*************************************************************************
	> File Name: handle_wave.h
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Tue 06 Dec 2016 04:10:36 PM CST
 ************************************************************************/

#ifndef _HANDLE_WAVE_H
#define _HANDLE_WAVE_H

typedef struct
{
    char chRIFF[4];                 // "RIFF" 标志
    int  total_Len;                 // 文件长度
    char chWAVE[4];                 // "WAVE" 标志
    char chFMT[4];                  // "fmt" 标志
    int  dwFMTLen;                  // 过渡字节（不定）  一般为16
    short fmt_pcm;                  // 格式类别
    short  channels;                // 声道数
    int fmt_samplehz;               // 采样率
    int fmt_bytepsec;               // 位速
    short fmt_bytesample;           // 一个采样多声道数据块大小
    short fmt_bitpsample;
   // 一个采样占的 bit 数
    char chDATA[4];                 // 数据标记符＂data ＂
    int  dwDATALen;                 // 语音数据的长度，比文件长度小42一般。这个是计算音频播放时长的关键参数~
}WaveHeader;

#endif
