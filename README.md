# AudioResamplerate 
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badge/)
[![stable](http://badges.github.io/stability-badges/dist/stable.svg)](http://github.com/badges/stability-badges)

## Introduction
This is an audio resamplerate program based on [Secret Rabbit Code](http://www.mega-nerd.com/SRC/index.html) and [iniparser](http://ndevilla.free.fr/iniparser).

## Component
- log : print log message.
- iniparser : configure the program by ``config.ini`` dynamically.
- resamplerate : resamplerate your parogram.

## Installation
```
$ git clone git@github.com:AnSwErYWJ/AudioResamplerate.git
```

## Usage
1. If you modify the file of three components, compile three dynamic library of components : 
    ```
    $ make log
    $ make iniparser
    $ make resamplerate
    ```
    
2. If you modify the ``source/resamplerate.c``, compile bin :
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

4. Then,run your program with :
    ```
    $ export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
    $ ./bin/resamplerate
    ```

## Environment
+ Linux
+ POSIX C
+ Bash Shell

## Todo
- [ ] Use TOML replace iniparser.

## Reference
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
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

    AudioResamplerate
    Copyright (C) 2016  AnSwErYWJ

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
