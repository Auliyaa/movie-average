#include <frame_handlers.h>

#include <iostream>
#include <sstream>

#include <QtGui/QImage>
#include <chrono>

#define DVAL(V) std::cout << #V << " = " << V << std::endl

std::string time_str(double time)
{
  std::string units = "secs";
  if (time >= 60)
  {
    time /= 60.;
    units = "mins";
  }
  if (time >= 60)
  {
    time /= 60.;
    units = "hours";
  }
  std::stringstream res;
  res << time << units;
  return res.str();
}

void avg_line_handler::init(AVFormatContext *format, AVCodecContext *codec, AVStream* stream)
{
  // Deduce frame count from duration and avg_frame_rate
  frame_count = stream->nb_frames;
  if (frame_count == 0)
  {
    frame_count = std::floor(((double)format->duration / (double)AV_TIME_BASE) * (double)stream->avg_frame_rate.num / (double)stream->avg_frame_rate.den);
  }

  frame_width  = codec->width;
  frame_height = codec->height;

  data = reinterpret_cast<uint8_t*>(malloc(frame_count * frame_height * 3));

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void avg_line_handler::handle(rgb_frame frame)
{
  if (frame.display_number >= frame_count)
  {
    return;
  }

  if (frame.display_number%(frame_count/200) == 0)
  {
    double now       = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    double took      = (now - start_time) / 1000.;
    double percent   = (double)frame.display_number * 100. / (double)frame_count;
    double fps       = (double)frame.display_number / (double)took;
    double remaining = ((double)took * ((double)frame_count - (double)frame.display_number)) / ((double)frame.display_number);

    std::cout << "Frame " << frame.display_number << " / " << frame_count << " (" << percent << "%)" << std::endl

              << " Elapsed  : " << time_str(took)                                         << std::endl
              << " FPS      : " << fps                                                    << std::endl
              << " Remaining: " << time_str(remaining)                                    << std::endl
              << std::endl;
  }

  for (size_t y=0; y < frame_height; ++y)
  {
    double ttl_r = 0;
    double ttl_g = 0;
    double ttl_b = 0;

    for (size_t x=0; x < frame_width; ++x)
    {
      rgb px = fetch_pixel(frame.data, x, y, frame_width);
      ttl_r += px.r;
      ttl_g += px.g;
      ttl_b += px.b;
    }

    rgb avg;
    avg.r = std::round(ttl_r / (double)frame_width);
    avg.g = std::round(ttl_g / (double)frame_width);
    avg.b = std::round(ttl_b / (double)frame_width);

    set_pixel(avg, data, frame.display_number, y, frame_count);
  }
}

void avg_line_handler::save(const std::string &filename) const
{
  QImage img = QImage(frame_count, frame_height, QImage::Format_RGB888);
  for (size_t x=0; x < frame_count; ++x)
  {
    for (size_t y=0; y < frame_height; ++y)
    {
      rgb px = fetch_pixel(data, x, y, frame_count);
      img.setPixel(x, y, qRgb(px.r, px.g, px.b));
    }
  }
  img.save(filename.c_str());
}

rgb fetch_pixel(uint8_t* data, size_t x, size_t y, size_t frame_width)
{
  size_t offset = x * 3 + y * frame_width * 3;
  rgb px;
  px.r = data[offset]; px.g = data[offset+1]; px.b = data[offset+2];
  return px;
}

void set_pixel(rgb px, uint8_t *data, size_t x, size_t y, size_t frame_width)
{
  size_t offset = x * 3 + y * frame_width * 3;
  data[offset]   = px.r;
  data[offset+1] = px.g;
  data[offset+2] = px.b;
}
