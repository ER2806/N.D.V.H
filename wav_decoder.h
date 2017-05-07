#ifndef WAV_DECODER_H
#define WAV_DECODER_H
#include "base_decoder.h"
#include "wav.h"

class wav_decoder : base_decoder
{
public:
    wav_decoder();
    ~wav_decoder();
    wav_decoder(const std::string& in_filename);
    const std::string decode();
};

#endif
