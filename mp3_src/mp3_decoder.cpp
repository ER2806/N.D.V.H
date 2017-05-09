#ifndef MP3_DECODER_CPP
#define MP3_DECODER_CPP

#include "mp3_decoder.h"


mp3_decoder::mp3_decoder(const std::string& in_filename)
    : base_decoder(in_filename)
{
}


void mp3_decoder::skip_byte(std::ifstream &in) {
    char bufbyte;
    try {
        in.get(bufbyte);
    }
    catch (std::istream::failure) {
        throw common_exception("File fail.");
    }
}

const std::string mp3_decoder::decode() {

    std::ifstream in(input_file, std::ios::in | std::ios::binary);

    std::vector <unsigned> bit_message;
    raw_header Header;

    for (unsigned k = 0; search_valid_header(in, Header); ++k) {
        if (k % sampling_frequency == 0) {
            header bufHeader(Header);

            unsigned to_skip = bufHeader.frame_size() + ((unsigned) bufHeader.crc_present()) * crc_bytes - 1;
            for (unsigned i = 0; i < to_skip; ++i) {
                skip_byte(in);
            }

            char coding_byte = 0;
            try {
                in.get(coding_byte);
            }
            catch (std::istream::failure) {
                throw common_exception("File fail.");
            }
            unsigned variable = (unsigned) ((coding_byte & 0x01) == 0x01);
            bit_message.push_back(variable);

            if (bit_message.size() % B == 0) {
                bool is_end_of_message = true;
                for (unsigned i = 0; i < B; ++i) {
                    if (bit_message[bit_message.size() - 1 - i] == 1) {
                        is_end_of_message = false;
                        break;
                    }
                }
                if (is_end_of_message) {
                    break;
                }
            }
        }
    }

    std::string message = create_message(bit_message);

    return message;

}

char mp3_decoder::create_symb(std::vector<unsigned>& code){

    unsigned result = 0;
    std::reverse(code.begin(), code.end());
    for (unsigned i = 0; i < code.size(); ++i) {
        result += code[i] * pow(2, i);
    }
    char result_symb = result;

    return result_symb;
}

std::string mp3_decoder::create_message(std::vector<unsigned>& code_message){

    std::string str_message;

    for (unsigned i = 0; i < code_message.size() - B; i += B) {
        std::vector <unsigned> buf;
        for (unsigned j = 0; j < B; ++j) {
            buf.push_back(code_message[i + j]);
        }
        char message_symb = create_symb(buf);
        str_message.push_back(message_symb);
    }

    return str_message;
}

bool mp3_decoder::search_valid_header(std::ifstream &in, raw_header& header){
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

#endif // MP3_DECODER_CPP
