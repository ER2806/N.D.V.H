#include "base_decoder.h"

base_decoder::base_decoder()
    : input_file("")
{
}


base_decoder::~base_decoder()
{
}


base_decoder::base_decoder(const std::string& in_filename)
    : input_file(in_filename)
{
}


void base_decoder::set_input_file(const std::string& in_filename)
{
    input_file = in_filename;
}
