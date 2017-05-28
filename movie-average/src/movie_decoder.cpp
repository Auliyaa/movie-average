#include <movie_decoder.h>

#include <iostream>
#include <sstream>

#define THROW_ERROR(MSG)\
  std::stringstream __err_ss__;\
  __err_ss__ << MSG;\
  throw std::runtime_error(__err_ss__.str())

frame_handler::~frame_handler()
{
}

movie_decoder::movie_decoder()
  : _frame_handler(nullptr),
    _format_context(NULL),
    _stream(NULL),
    _codec_context(NULL),
    _codec_params(NULL),
    _codec(NULL),
    _sws_context(NULL)
{
  static bool avcodec_registered = false;
  if (!avcodec_registered)
  {
    avcodec_registered = true;
    av_register_all();
  }
}

movie_decoder::~movie_decoder()
{
  delete _frame_handler;
}

void movie_decoder::open(const std::string& filename)
{
  // Open video file
  if (avformat_open_input(&_format_context, filename.c_str(), 0, NULL) != 0)
  {
    THROW_ERROR("Could not open input file");
  }

  // Retreive stream informations
  if(avformat_find_stream_info(_format_context, NULL) < 0)
  {
    THROW_ERROR("Could not find stream informations");
  }

  // Find the first video stream
  unsigned int stream_index = 0;
  bool stream_found = false;
  for(stream_index=0; stream_index < _format_context->nb_streams; ++stream_index)
  {
    if (_format_context->streams[stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      stream_found = true;
      break;
    }
  }
  if (!stream_found)
  {
    THROW_ERROR("Could not find any video stream");
  }

  _stream = _format_context->streams[stream_index];
  _codec_params = _stream->codecpar;

  // Find the associated video decoder
  _codec = avcodec_find_decoder(_codec_params->codec_id);
  if (_codec == NULL)
  {
    THROW_ERROR("No codec found for stream #" << _stream->id);
  }

  // Fetch the codec context
  _codec_context = avcodec_alloc_context3(_codec);
  if (_codec_context == NULL)
  {
    THROW_ERROR("Can not fetch context for codec with id " << _codec_params->codec_id);
  }

  // Setup the codec context
  if(_codec->capabilities & CODEC_CAP_TRUNCATED)
  {
    _codec_context->flags |= CODEC_FLAG_TRUNCATED;
  }
  avcodec_parameters_to_context(_codec_context, _codec_params);

  // Open the codec
  if (avcodec_open2(_codec_context, _codec, NULL) < 0)
  {
    THROW_ERROR("Can not open codec with id " << _codec_params->codec_id);
  }

  // And init SWS to transform frames into RGB888 images
  _sws_context = sws_getContext(_codec_params->width, _codec_params->height,
                                _codec_context->pix_fmt, _codec_params->width, _codec_params->height,
                                AV_PIX_FMT_RGB24, 0, NULL, NULL, NULL);
}

bool movie_decoder::is_open() const
{
  return _codec != NULL;
}

void movie_decoder::close()
{
  if (!is_open())
  {
    return;
  }
}

void movie_decoder::decode_file(size_t thread_count)
{
  _pool.resize(thread_count);

  for(size_t ii=0; ii < thread_count; ++ii)
  {
    _pool.push([this](int)
    {
      try
      {
        while(true)
        {
          std::vector<AVFrame*> frames = next_frame();

          if (frames.empty())
          {
            break;
          }

          for (AVFrame* frame : frames)
          {
            _frame_handler->handle(frame);
          }

          std::this_thread::yield();
        }
      } catch(std::exception& ex)
      {
        std::cerr << ex.what() << std::endl;
      }
    });
  }
  for(int ii=0; ii < _pool.size(); ++ii)
  {
    _pool.get_thread(ii).join();
  }
}

void movie_decoder::set_handler(frame_handler* handler)
{
  _frame_handler = handler;
}

std::vector<AVFrame*> movie_decoder::next_frame()
{
  std::lock_guard<spin_lock> __guard__(_lock);

  std::vector<AVFrame*> result;

  AVPacket packet;
  av_init_packet(&packet);

  while (av_read_frame(_format_context, &packet) == 0)
  {
    if (packet.stream_index != _stream->index)
    {
      av_packet_unref(&packet);
      continue;
    }

    if (avcodec_send_packet(_codec_context, &packet) != 0)
    {
      THROW_ERROR("Failed to send packet");
    }

    av_packet_unref(&packet);

    AVFrame* frame = av_frame_alloc();
    while(avcodec_receive_frame(_codec_context, frame) == 0)
    {
      result.push_back(frame);
    }
    break;
  }

  return result;
}
