# AudioResamplerate 
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badge/)
[![stable](http://badges.github.io/stability-badges/dist/stable.svg)](http://github.com/badges/stability-badges)

## Introduction
This is an audio resamplerate program for Linux,support PCM and WAV audio(mone or stereo),bits of each sample is **16** default.

## Component
- log : print verbose/info/debug/warning/error message.
- iniparser : configure the parameters by ``config.ini`` dynamically.
- resamplerate : resamplerate your parogram.

## Installation
```
$ git clone git@github.com:AnSwErYWJ/AudioResamplerate.git
```

## Usage
1. If you modify the file of three components, compile three dynamic library of components : 
    ```
    $ make iniparser
    $ make resamplerate
    ```
    
2. If you modify the ``source/resamplerate.c`` ``include/resamplerate``, compile bin :
	```
	$ make
	``` 

3. Modify the ``config.ini`` to configure the parameters, input support PCM or WAV audio files, and output only support PCM audio files, such as :
	```
	[audio]
	channels = 1;
	input_sample_rate = 16000;
	output_sample_rate = 48000;

	[path]
	input = ./audio/S16bit-LE-16kHz-Mono.pcm;
	output = ./audio/out.pcm;

	[type]
	input = wav;
	output = null;
	``` 

4. Then,run the program with :
    ```
    $ export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
    $ ./resamplerate
    ```

## Environment
+ Linux
+ POSIX C
+ Bash Shell

## Todo
- [ ] Use TOML replace iniparser.

## Reference
- [log](https://github.com/AnSwErYWJ/DogFood/tree/master/C/log)
- [Secret Rabbit Code](http://www.mega-nerd.com/SRC/index.html)
- [iniparser](http://ndevilla.free.fr/iniparser)
- [Convering 8/16/32 bits/sample array to floats-array](http://stackoverflow.com/questions/4632502/waveinproc-windows-audio-question)

## About me
[![forthebadge](http://forthebadge.com/images/badges/ages-20-30.svg)](http://forthebadge.com)
- WebSite：[http://www.answerywj.com](http://www.answerywj.com)
- Email：[yuanweijie1993@gmail.com](https://mail.google.com)
- GitHub：[AnSwErYWJ](https://github.com/AnSwErYWJ)
- Blog：[AnSwEr不是答案的专栏](http://blog.csdn.net/u011192270)
- Weibo：[@AnSwEr不是答案](http://weibo.com/1783591593)

## Copyright and License
BSD 2-Clause License

Copyright (c) 2016-2018, AnSwErYWJ(Weijie Yuan),yuanweijie1993@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
