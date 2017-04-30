#ifndef WAV_DECODER_H
#define WAV_DECODER_H
#include "QString"
#include "base_decoder.h"
#include "wav.h"

class wav_decoder : base_decoder
{
public:
    wav_decoder();
    ~wav_decoder();
    wav_decoder(const QString& in_filename);
    wav_decoder(const std::string& in_filename);
    const QString decode();
};

#endif // STEALTH_H
