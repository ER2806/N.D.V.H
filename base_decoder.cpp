#include "base_decoder.h"

base_decoder::base_decoder()
    : base_coder()
{
}


base_decoder::~base_decoder()
{
}


base_decoder::base_decoder(const QString& in_filename)
    : base_coder(in_filename)
{
}

base_decoder::base_decoder(const std::string& in_filename)
    : base_coder(in_filename)
{
}
