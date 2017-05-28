#pragma once

#include <functional>
#include <ctpl_stl.h>
#include <spin_lock.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

struct frame_handler
{
  virtual ~frame_handler();
  virtual void handle(AVFrame* frame)=0;
};

class movie_decoder
{
  std::atomic_bool   _eof;
  spin_lock          _lock;

  frame_handler*     _frame_handler;

  AVFormatContext*   _format_context;
  AVStream*          _stream;

  AVCodecContext*    _codec_context;
  AVCodecParameters* _codec_params;
  AVCodec*           _codec;

  SwsContext*        _sws_context;

  ctpl::thread_pool  _pool;

public:
  movie_decoder();
  virtual ~movie_decoder();

  void open(const std::string& filename);
  bool is_open() const;
  void close();

  void decode_file(size_t thread_count=std::thread::hardware_concurrency());

  void set_handler(frame_handler* handler);

private:
  std::vector<AVFrame*> next_frame();
};
