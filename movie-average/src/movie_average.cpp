#include <iostream>
#include <tclap/CmdLine.h>

#include <movie_decoder.h>
#include <frame_handlers.h>

int main(int argc, char **argv)
{
  // =========================================
  // Parse arguments
  // =========================================
  std::string input;
  std::string output;

  try
  {
    TCLAP::CmdLine               cmd("Averages a movie colors in a single picture", ' ', "1.0.0");
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

  std::cout << "Startng decoding of  : " << input << std::endl;
  std::cout << "> Saving results into: " << output << std::endl;

  movie_decoder decoder;
  decoder.open(input);

  avg_line_handler* hdlr = new avg_line_handler();
  decoder.set_handler(hdlr);
  decoder.decode_file();
  hdlr->save(output);

  return 0;
}

