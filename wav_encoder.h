#ifndef WAV_ENCODER_H
#define WAV_ENCODER_H
#include "QString"
#include "base_encoder.h"
#include "wav.h"

class wav_encoder : public base_encoder
{
public:
    wav_encoder();
    ~wav_encoder();
    wav_encoder(const QString& in_filename, const QString& out_filename, const QString& msg);
    wav_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg);
    const int encode();
};

#endif // STEALTH_H
