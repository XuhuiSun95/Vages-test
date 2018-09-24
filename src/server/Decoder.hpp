#ifndef DECODER_HPP
#define DECODER_HPP

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}
#include <string>
#include <cstring>
#include <iostream>
#include <atomic>

class Decoder {

public:

    static Decoder* Instance(const std::string& file);
    static void Release();

    void Run();
    int GetCount();

private:

    static Decoder* sInstance;

    const char* src_filename;
    AVFormatContext* fmt_ctx;
    AVCodecContext* video_dec_ctx;
    AVStream* video_stream;
    int width, height;
    enum AVPixelFormat pix_fmt;

    uint8_t* video_dst_data[4];
    int video_dst_linesize[4];
    int video_dst_bufsize;

    int video_stream_idx;
    AVPacket pkt;
    AVFrame* frame;
    std::atomic<int> video_frame_count;
    int output_count;
    
    Decoder(const std::string& file);
    ~Decoder();

    void Init();
    int open_codec_context(int* stream_idx, AVCodecContext** dec_ctx, AVFormatContext* fmt_ctx, enum AVMediaType type);
    int decode_packet(int* got_frame, int cached);
    void pgm_save(unsigned char*buf, int warp, int xsize, int ysize, char* filename);
};

#endif
