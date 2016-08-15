#!/bin/bash

export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

./bin/resamplerate ./audio/S16bit-LE-16kHz-Mono.pcm ./audio/out.pcm
