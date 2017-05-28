#pragma once

#include <movie_decoder.h>

struct rgb
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

rgb fetch_pixel(uint8_t* data, size_t x, size_t y, size_t frame_width);
void set_pixel(rgb px, uint8_t* data, size_t x, size_t y, size_t frame_width);

struct avg_line_handler: public frame_handler
{
  size_t   frame_count;
  size_t   frame_width;
  size_t   frame_height;
  uint8_t* data;

  void init(AVFormatContext *format, AVCodecContext *codec, AVStream *stream);
  void handle(rgb_frame frame);

  void save(const std::string& filename) const;
};
