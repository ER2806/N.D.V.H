#include "base_encoder.h"


base_encoder::base_encoder()
{
    input_file = "";
    output_file = "";
    message = "";
}


base_encoder::~base_encoder()
{
}


base_encoder::base_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
    : input_file(in_filename) ,
      output_file(out_filename),
      message(msg)
{
}


void base_encoder::set_message(const std::string& msg)
{
    message = msg;
}


void base_encoder::set_output_file(const std::string& out_filename)
{
    output_file = out_filename;
}


void base_encoder::set_input_file(const std::string& in_filename)
{
    input_file = in_filename;
}
