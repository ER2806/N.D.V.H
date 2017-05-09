#ifndef ENCODER_JPG_H
#define ENCODER_JPG_H
/*

#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <csetjmp>
#include <string>
#include <vector>
#include <bitset>
#include "base_encoder.h"
#include "base_exception.h"



class jpg_encoder: public base_encoder
{
public:
    jpg_encoder();
    jpg_encoder(std::string& in_filename, std::string& out_filename, std::string& msg);
    ~jpg_encoder();

    virtual void encode() override;
private:
    std::string key = "Hello bro";
    size_t DCT_coefs_width;
    size_t DCT_coefs_height;
    bool is_decompressed = false;
    size_t variable_elem_index = 10;                 //Индекс элемента в который запишем один бит
    std::vector<std::bitset<8>> bitset_y_components; // Синие биты цветов
    // structs from jpeglib
    struct jpeg_decompress_struct cinfo;
    //struct jpeg_error_mgr jerr;
    jvirt_barray_ptr *coeffs_array;
    jpeg_component_info* compptr_one;              //Информация о массиве компонент Y (CrCb)

    FILE * infile = NULL;
    FILE * outfile = NULL;
    JBLOCKARRAY DCT_coefficients;                  // Коэффициенты ДКП


    //Private methods
    void close_input_image();
    void read_jpeg_file();
    void create_jpeg_file();
    void get_bitset_y_components();
    void merge_bitset_y_comp_with_image();
    size_t get_header_bits_amount();
    size_t get_message_bits_amount();
    bool check_capacity();
    void add_header_to_bitset();
    void add_message_to_bitset();
    void add_char_bits_to_bitset(std::bitset<8>& elem, size_t& tmp_index);
    void add_int_bits_to_bitset(std::bitset<32>& elem, size_t& tmp_index);
    char bitset_to_char(std::bitset<8>& a);
    struct jpegErrorManager {
        /* "public" fields */
/*
        struct jpeg_error_mgr pub;
        /* for return to caller */
/*
        jmp_buf setjmp_buffer;
    };

};
*/
#endif // ENCODER_JPG_H
