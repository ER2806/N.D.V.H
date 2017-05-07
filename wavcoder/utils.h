#ifndef UTILS_HPP
#define UTILS_HPP
#include "format_parser.h"
#include "formats.h"

#include "wav_src/wav_decoder.h"
#include "mp3_src/mp3_decoder.h"
//#include "bmp_src/bmp_decoder.h"
//#include "jpg_src/jpg_decoder.h"
//#include "png_src/png_decoder.h"
#include "wav_src/wav_encoder.h"
#include "mp3_src/mp3_encoder.h"
//#include "bmp_src/bmp_encoder.h"
//#include "jpg_src/jpg_encoder.h"
//#include "png_src/png_encoder.h"

#include <fstream>
#include <iostream>
#include <ctype.h>
#include <string>

const std::string get_message(std::ifstream &txt_file);
const bool encode(std::string input_file, std::string output_file, std::string msg_txt_file);
const bool msg_to_file(const std::string& message, const std::string& file_name);
const bool decode(const std::string& input_file, const std::string& output_file);

#endif // UTILS_HPP
