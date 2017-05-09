#ifndef DECODER_BMP_H
#define DECODER_BMP_H


#include <QTGui>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>
#include "base_decoder.h"
#include "base_exception.h"

class bmp_decoder: public base_decoder
{
public:
    bmp_decoder();
    bmp_decoder(const std::string& path);
    void set_input_file(const std::string& in_filename);
    ~bmp_decoder() = default;

    virtual const std::string decode() override;

private:
    QImage image;
    size_t image_width;
    size_t image_height;
    size_t key_len = 9;
    std::vector<std::bitset<8>> bitset_blue_colors; // Синие биты цветов
    std::string header;
    std::string message;
    size_t len_msg;
    size_t step;

    //Private methods
    void get_bitset_blue_colors();
    size_t get_header_bits_amount();
    void get_header();
    void get_char_from_image(std::bitset<8>&, size_t&, size_t);
    void get_len_message(std::bitset<32>&, size_t&);
    void get_step_message(std::bitset<8>&, size_t&);
    bool is_valid_image();
    void get_message_from_image();
};

#endif // DECODER_BMP_H
