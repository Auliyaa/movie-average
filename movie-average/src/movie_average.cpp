#include <iostream>
#include <tclap/CmdLine.h>

#include <movie_decoder.h>

struct my_handler: public frame_handler
{
  void handle(AVFrame* frame)
  {
//    std::cout << "Frame #" << frame->display_picture_number << std::endl;
  }
};

int main(int argc, char **argv)
{
  // =========================================
  // Parse arguments
  // =========================================
  std::string input;
  std::string output;

  try
  {
    TCLAP::CmdLine               cmd("Averages a movie colors in a single picture", ' ', AIO_PROJECT_VERSION);
    TCLAP::ValueArg<std::string> input_arg("i", "input", "Input file", true, "", "string");
    TCLAP::ValueArg<std::string> output_arg("o", "output", "Output file", true, "", "string");
    cmd.add(input_arg);
    cmd.add(output_arg);
    cmd.parse(argc,argv);
    input = input_arg.getValue();
    output = output_arg.getValue();
  } catch(TCLAP::ArgException& ex)
  {
    std::cerr << "Arguments parse error: " << ex.what() << std::endl;
    return 1;
  }

  movie_decoder decoder;
  decoder.open(input);
  decoder.set_handler(new my_handler);
  decoder.decode_file();

  return 0;
}

