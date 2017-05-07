#include "decoder_bmp.h"

DecoderBMP::DecoderBMP(): base_decoder(){
}

DecoderBMP::DecoderBMP(const std::string& path): base_decoder(path){
}


void DecoderBMP::get_bitset_blue_colors()
{
    QColor tmp;
    for (size_t i = 0; i < image_width; i++){
        for (size_t j = 0; j < image_height; j++){
            tmp = image.pixel(i, j);
            bitset_blue_colors.push_back(tmp.blue());
        }
    }
}


const std::string DecoderBMP::decode(){
    if (input_file.empty()){
        throw common_exception("The file is not indicate");
    }
    image = QImage(input_file.c_str());
    if (image.isNull())
        throw common_exception("Image not foung or image was not opened");
    image_height = image.height();
    image_width = image.width();
    get_bitset_blue_colors();
    get_header();
    //std::cout << header << std::endl;
    if (!is_valid_image()){
        throw common_exception("This image does not have additional information");
    }
    get_message_from_image();
    return message;
}

size_t DecoderBMP::get_header_bits_amount()
{
    size_t amount = key_len + 4 + 1 + 8;
    return amount * 8;
}


void DecoderBMP::get_header(){
    size_t bits_amout = get_header_bits_amount() * 2;
    if (image_width * image_height < bits_amout){
        throw common_exception("This image does not contain the additional information");
    }
    std::bitset<8> tmp_uchar;
    std::bitset<32> len;
    size_t bit_position = 4;
    for (size_t i = 0; i < 4 + key_len; i++){
        get_char_from_image(tmp_uchar, bit_position, 4);
        header.push_back(static_cast<char>(tmp_uchar.to_ulong()));
    }
    get_len_message(len, bit_position);
    len_msg = len.to_ulong();

    // Получим 2 символа @@
    for (int i = 0; i < 2; i++){
        get_char_from_image(tmp_uchar, bit_position, 4);
        header.push_back(static_cast<char>(tmp_uchar.to_ulong()));
    }

    // Получим шаг
    get_char_from_image(tmp_uchar, bit_position, 4);
    step = tmp_uchar.to_ulong();


    // Получим 2 символа @@
    for (int i = 0; i < 2; i++){
        get_char_from_image(tmp_uchar, bit_position, 4);
        header.push_back(static_cast<char>(tmp_uchar.to_ulong()));
    }
}

void DecoderBMP::get_char_from_image(std::bitset<8>& tmp_uchar, size_t& position, size_t step){
    for (int i = 0; i < 4; i++){
        tmp_uchar[2*i] = bitset_blue_colors[i * step  + position][0];
        tmp_uchar[2*i + 1] = bitset_blue_colors[i * step + position][1];
    }
    position += step * 4;
}


void DecoderBMP::get_len_message(std::bitset<32> &len_bits, size_t &position)
{
    for (int i = 0; i < 16; i++){
        len_bits[2*i] = bitset_blue_colors[i * 4  + position][0];
        len_bits[2*i + 1] = bitset_blue_colors[i * 4 + position][1];
    }

    position += 64;
}


bool DecoderBMP::is_valid_image(){
    if (header.empty())
        get_header();
    // количество байтов в которых хранится хедер + отступы с двух сторон
    size_t header_bits = get_header_bits_amount() * 2 + 4 + 4;
    // количество байтов в которых хранится сообщение
    size_t message_bits = len_msg * 4 * step;
    if (message_bits + header_bits > image_width*image_height){
        return false;
    }

    return header == "//Hello bro##@@@@";
}


void DecoderBMP::get_message_from_image()
{
    std::bitset<8> tmp_uchar;
    size_t bit_position = (get_header_bits_amount() * 2) + 4;
    for (size_t i = 0; i < len_msg; i++){
        get_char_from_image(tmp_uchar, bit_position, step);
        message.push_back(static_cast<char>(tmp_uchar.to_ulong()));
    }
}
