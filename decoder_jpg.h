#ifndef DECODER_JPG_H
#define DECODER_JPG_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <jpeglib.h>
#include "base_decoder.h"
#include "base_exception.h"
#include <csetjmp>

class DecoderJPG: public base_decoder
{
public:
    DecoderJPG();
    DecoderJPG(std::string& in_filename);
    ~DecoderJPG() = default;

    virtual const std::string decode() override;

private:
    size_t key_len = 9;
    size_t variable_elem_index = 10;                 //Индекс элемента в который записан один бит
    std::vector<std::bitset<8>> bitset_y_components; // <Биты Y комнонент
    size_t DCT_coefs_width;
    size_t DCT_coefs_height;
    std::string header;
    std::string message;
    size_t len_msg;

    // private methods
    void get_bitset_y_comp_from_file();
    void get_header_from_bitset();
    void get_message_from_bitset();
    size_t get_header_bits_amount();
    size_t get_message_bits_amount();
    bool is_valid_capacity_without_message();
    bool is_valid_capacity_with_message();
    bool is_valid_image();
    void get_char_from_bitset(std::bitset<8>& tmp_uchar, size_t& position, size_t step);
    void get_len_message_from_bitset(std::bitset<32> &len_bits, size_t &position);

    char bitset_to_char(std::bitset<8>& a);

    struct jpegErrorManager {
        /* "public" fields */
        struct jpeg_error_mgr pub;
        /* for return to caller */
        jmp_buf setjmp_buffer;
    };
};
#endif // DECODER_JPG_H
