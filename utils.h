#ifndef UTILS_HPP
#define UTILS_HPP
#include "formats.h"

#include "wav_src/wav_decoder.h"
#include "mp3_src/mp3_decoder.h"
#include "image_src/bmp_decoder.h"
#include "image_src/jpg_decoder.h"
#include "wav_src/wav_encoder.h"
#include "mp3_src/mp3_encoder.h"
#include "image_src/bmp_encoder.h"
#include "image_src/jpg_encoder.h"

#include <fstream>
#include <iostream>
#include <ctype.h>
#include <string>

const bool encode(const std::string& input_file, const std::string& output_file, const std::string& msg_txt_file);
const bool decode(const std::string& input_file, const std::string& output_file);

const enum formats _get_format(const std::string& filename);
const std::string _get_message(std::ifstream& txt_file);
const bool _msg_to_file(const std::string& message, const std::string& file_name);

#endif // UTILS_HPP
