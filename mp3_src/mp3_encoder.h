#ifndef MP3_ENCODER_H
#define MP3_ENCODER_H

#include <fstream>
#include <bitset>
#include <vector>
#include <algorithm>

#include "base_encoder.h"
#include "head.h"
#include "io.h"

class mp3_encoder : public base_encoder
{
public:
    mp3_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg);
    void encode();
private:
    void rewind_file(std::ifstream& in);
    bool is_acceptable_message();
    unsigned number_of_frames(std::ifstream& in);
    std::vector<unsigned> rebuild_message();
    std::vector<unsigned> rebuild_symb(char symb);
    void skip_byte(std::ifstream &in, std::ofstream &out);

    bool search_valid_header(std::ifstream &in, std::ofstream& out, raw_header& header);
    bool search_valid_header(std::ifstream &in, raw_header& header);
};

#endif // MP3_ENCODER_H
