# Vegas-test
编写一个TCP Server和Client。Server 调用ffmpeg库解码H.264视频按固定时间频率获取视频帧，并把获取的图片发送给连接到Server的多个client。

## Setup
FFmeg Mac OS dynamic libs and Makefile is included, run `make` to compile on Mac OS. For Linux and Windows system, you may want to change the libs accordingly.

## Usage
tcp server:
```
./server_tcp <inputfile>
```

tcp client:
```
./client_tcp <id>
```
