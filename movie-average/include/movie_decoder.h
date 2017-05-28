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

struct rgb_frame
{
  int height;
  int width;
  size_t display_number;

  uint8_t* data;
};

struct frame_handler
{
  virtual ~frame_handler();

  virtual void init(AVFormatContext* format, AVCodecContext* codec)=0;
  virtual void handle(rgb_frame frame)=0;
};

class movie_decoder
{
  size_t             _current_frame;

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

//  void decode_file(size_t thread_count=std::thread::hardware_concurrency());
  void decode_file(size_t thread_count=1);

  void set_handler(frame_handler* handler);

private:
  std::vector<AVFrame *> next_frame();
};
