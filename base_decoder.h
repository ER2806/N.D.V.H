#ifndef BASE_DECODER_HPP
#define BASE_DECODER_HPP
#include "base_coder.h"

class base_decoder : public base_coder
{
public:
    base_decoder();
    virtual ~base_decoder();
    base_decoder(const QString& in_filename);
    base_decoder(const std::string& in_filename);
    virtual const QString decode() = 0;
};

#endif // BASE_DECODER_HPP
