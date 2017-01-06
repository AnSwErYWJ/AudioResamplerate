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
    char  riff_id[4];                       //"RIFF"
    int   riff_datasize;                    // RIFF chunk data size,exclude riff_id[4] and riff_datasize,total - 8

    char  riff_type[4];                     // "WAVE"

    char  fmt_id[4];                        // "fmt "
    int   fmt_datasize;                     // fmt chunk data size,16 for pcm
    short fmt_compression_code;             // 1 for PCM
    short fmt_channels;                     // 1(mono) or 2(stereo)
    int   fmt_sample_rate;                  // samples per second
    int   fmt_avg_bytes_per_sec;            // sample_rate * channels * bit_per_sample / 8
    short fmt_block_align;                  // number bytes per sample, bit_per_sample * channels / 8
    short fmt_bit_per_sample;               // bits of each sample(8,16,32).

    char  data_id[4];                       // "data"
    int   data_datasize;                    // data chunk size,pcm_size - 44
}WaveHeader_t;

void init_wavheader(WaveHeader_t *wavheader);
int read_wavheader(FILE *fp,WaveHeader_t *wavheader);
int write_wavheader(FILE *fp,WaveHeader_t wavheader);
void print_wavheader(WaveHeader_t wavheader);

#endif
