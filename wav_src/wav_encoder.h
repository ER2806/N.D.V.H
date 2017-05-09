#ifndef WAV_ENCODER_H
#define WAV_ENCODER_H
#include "base_encoder.h"
#include "base_exception.h"

class wav_encoder : public base_encoder
{
public:
    wav_encoder();
    ~wav_encoder();
    wav_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg);
    void encode();
};

#endif // STEALTH_H
