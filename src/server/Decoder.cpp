#include "Decoder.hpp"

Decoder* Decoder::sInstance = nullptr;

Decoder* Decoder::Instance(const std::string& file) {

    if(sInstance==nullptr)
        sInstance = new Decoder(file);

    return sInstance;
}

void Decoder::Release() {

    delete sInstance;
    sInstance = nullptr;
}

Decoder::Decoder(const std::string& file) {

    src_filename = file.c_str();
    fmt_ctx = NULL;
    video_dec_ctx = NULL;
    video_stream = NULL;

    *video_dst_data = {NULL};

    video_stream_idx = -1;
    frame = NULL;
    video_frame_count = 0;
    output_count = 0;
}

Decoder::~Decoder() {

    avcodec_free_context(&video_dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_frame_free(&frame);
    av_free(video_dst_data[0]);
}

void Decoder::Run() {

    int ret = 0, got_frame;

    Decoder::Init();
    if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];

        /* allocate image where the decoded image will be put */
        width = video_dec_ctx->width;
        height = video_dec_ctx->height;
        pix_fmt = video_dec_ctx->pix_fmt;
        ret = av_image_alloc(video_dst_data, video_dst_linesize,
                             width, height, pix_fmt, 1);
        if (ret < 0) {
            fprintf(stderr, "Could not allocate raw video buffer\n");
            return;
        }
        video_dst_bufsize = ret;
    }

    /* dump input information to stderr */
    av_dump_format(fmt_ctx, 0, src_filename, 0);

    if (!video_stream) {
        fprintf(stderr, "Could not find audio stream in the input, aborting\n");
        ret = 1;
        return;
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        return;
    }

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    /* read frames from the file */
    //while (av_read_frame(fmt_ctx, &pkt) >= 0) {
    while (av_seek_frame(fmt_ctx, -1, 5*output_count++*AV_TIME_BASE, AVSEEK_FLAG_BACKWARD)>=0 && av_read_frame(fmt_ctx, &pkt) >= 0) {

        if(output_count>20)
            break;
        AVPacket orig_pkt = pkt;
        do {
            ret = decode_packet(&got_frame, 0);
            if (ret < 0)
                break;
            pkt.data += ret;
            pkt.size -= ret;
        } while (pkt.size > 0);
        av_packet_unref(&orig_pkt);
    }

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame, 1);
    } while (got_frame);

    std::cout << "Decode succeeded" << std::endl;
}

int Decoder::GetCount() {

    return video_frame_count;
}

void Decoder::Init() {

    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, src_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", src_filename);
        return;
    }

    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        return;
    }
}

int Decoder::open_codec_context(int* stream_idx, AVCodecContext** dec_ctx, AVFormatContext* fmt_ctx, enum AVMediaType type)
{
    int ret, stream_index;
    AVStream *st;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders, with or without reference counting */
        av_dict_set(&opts, "refcounted_frames", "0", 0);
        if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }

    return 0;
}

int Decoder::decode_packet(int* got_frame, int cached) {

    char buf[1024];
    int ret = 0;
    int decoded = pkt.size;

    *got_frame = 0;

    if (pkt.stream_index == video_stream_idx) {
        /* decode video frame */
        ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
            return ret;
        }

        if (*got_frame) {

            //if (frame->width != width || frame->height != height ||
            //    frame->format != pix_fmt) {
            //    /* To handle this change, one could call av_image_alloc again and
            //     * decode the following frames into another rawvideo file. */
            //    fprintf(stderr, "Error: Width, height and pixel format have to be "
            //            "constant in a rawvideo file, but the width, height or "
            //            "pixel format of the input video changed:\n"
            //            "old: width = %d, height = %d, format = %s\n"
            //            "new: width = %d, height = %d, format = %s\n",
            //            width, height, av_get_pix_fmt_name(pix_fmt),
            //            frame->width, frame->height,
            //            av_get_pix_fmt_name(frame->format));
            //    return -1;
            //}

            //printf("video_frame%s n:%d coded_n:%d\n",
            //       cached ? "(cached)" : "",
            //       video_frame_count++, frame->coded_picture_number);
            printf("saving frame %3d\n", video_dec_ctx->frame_number);
            fflush(stdout);

            /* copy decoded frame to destination buffer:
             * this is required since rawvideo expects non aligned data */
            av_image_copy(video_dst_data, video_dst_linesize,
                          (const uint8_t **)(frame->data), frame->linesize,
                          pix_fmt, width, height);

            /* write to pgm file */
            snprintf(buf, sizeof(buf), "img_processed/output-%d.pgm", video_dec_ctx->frame_number);
            pgm_save(video_dst_data[0], video_dst_linesize[0], width, height, buf);
            video_frame_count = video_dec_ctx->frame_number;
        }
    }

    return decoded;
}

void Decoder::pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *f;
    int i;

    f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}
