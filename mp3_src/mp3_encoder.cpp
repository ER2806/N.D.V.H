#include "mp3_encoder.h"


mp3_encoder::mp3_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
    : base_encoder(in_filename, out_filename, msg)
{
}


void mp3_encoder::rewind_file(std::ifstream& in) {
    in.close();
    in.open(input_file, std::ios::in | std::ios::binary);
}

bool mp3_encoder::is_acceptable_message() {

    std::ifstream in(input_file, std::ios::in | std::ios::binary);

    unsigned frame_num = number_of_frames(in);
    if (frame_num <= 0) {
        throw common_exception("No valid frames in mp3-file.");
    }
    rewind_file(in);
    unsigned message_size = message.size() + 1; // for \0

    unsigned valid_message_size = frame_num / (B * sampling_frequency);
    if (valid_message_size >= message_size)
        return true;

    return false;
}

void mp3_encoder::skip_byte(std::ifstream &in, std::ofstream &out) {
    char bufbyte;
    try {
        in.get(bufbyte);
        out.put(bufbyte);
    }
    catch (std::istream::failure) {
        throw common_exception("File fail.");
    }
}

void mp3_encoder::encode() {

    if (!is_acceptable_message()) {
        throw common_exception("Is not acceptable message.");
    }

    std::ifstream in(input_file, std::ios::in | std::ios::binary);
    std::ofstream out(output_file, std::ios::out | std::ios::binary);

    std::vector <unsigned> bit_message = rebuild_message();
    unsigned size_message = bit_message.size();
    unsigned step = 0;

    raw_header Header;

    for (unsigned k = 0; search_valid_header(in, out, Header); ++k) {
        if (step < size_message && k % sampling_frequency == 0) {
            header bufHeader(Header);

            //size of frame + 2 bytes for crc  - 1 (which we need)
            unsigned to_skip = bufHeader.frame_size() + ((unsigned) bufHeader.crc_present()) * crc_bytes - 1;
            for (unsigned i = 0; i < to_skip; ++i) {
                skip_byte(in, out);
            }

            char coding_byte = 0;
            try {
                in.get(coding_byte);
            }
            catch (std::istream::failure) {              
                throw common_exception("File fail.");
            }

            unsigned significant_bit = bit_message[step];
            step++;

            if (significant_bit == 1){
                coding_byte = coding_byte | 0x01;
            }
            else {
                coding_byte = coding_byte & 0xFE;
            }

            out.put(coding_byte);
        }
    }

}

unsigned mp3_encoder::number_of_frames(std::ifstream& in) {
    unsigned result = 0;
    raw_header header;
    for (;search_valid_header(in, header); ++result) {
    }
    return result;
}

std::vector<unsigned> mp3_encoder::rebuild_symb(char symb) {

    unsigned symbint = symb;
    std::vector<unsigned> bitint;
    for (unsigned i = 0; i < B ; i++){
       bitint.push_back(std::bitset<B>(symbint)[i]);
    }
    std::reverse(bitint.begin(), bitint.end());

    return bitint;
}

std::vector<unsigned> mp3_encoder::rebuild_message(){


    std::vector<unsigned> result_message;

    for (unsigned i = 0; i < message.size(); ++i) {
        std::vector<unsigned> buf = rebuild_symb(message[i]);
        for (unsigned j = 0; j < B; ++j) {
            result_message.push_back(buf[j]);
        }
    }

    for (unsigned i = 0; i < B; ++i) { // \0 symbol
        result_message.push_back(0);
    }

    return result_message;
}

bool mp3_encoder::search_valid_header(std::ifstream &in, std::ofstream& out, raw_header& header){
    in >> header;
    out << header;

    while (!header.is_valid_header() && !in.fail()) {
        try {
            header.read_byte(in);
            out.put(header[3]);
        }
        catch (std::istream::failure) {
            throw common_exception("File fail.");
        }
    }

    return !in.fail();
}

bool mp3_encoder::search_valid_header(std::ifstream &in, raw_header& header){
    in >> header;

    while (!header.is_valid_header() && !in.fail()) {
        try {
            header.read_byte(in);
        }
        catch (std::istream::failure) {
            throw common_exception("File fail.");
        }
    }

    return !in.fail();
}


