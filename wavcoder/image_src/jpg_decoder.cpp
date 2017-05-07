#include "jpg_decoder.h"
#include <csetjmp>

void jpegErrorExit ( j_common_ptr cinfo )
{
    char jpegLastErrorMsg[JMSG_LENGTH_MAX];
    /* Create the message */
    ( *( cinfo->err->format_message ) ) ( cinfo, jpegLastErrorMsg );

    /* Jump to the setjmp point */
    throw common_exception( jpegLastErrorMsg ); // or your preffered exception ...
}

DecoderJPG::DecoderJPG()
             :base_decoder(){
}

DecoderJPG::DecoderJPG(std::string& in_filename)
             :base_decoder(in_filename){
}


const std::string DecoderJPG::decode(){
    get_bitset_y_comp_from_file();
    get_header_from_bitset();
    if (!is_valid_image()){
        throw common_exception("This image does not have additional information");
    }
    get_message_from_bitset();
    return message;
}

void DecoderJPG::get_bitset_y_comp_from_file()
{
    struct jpeg_decompress_struct cinfo;
    //struct jpeg_error_mgr jerr;
    jpegErrorManager jerr;
    FILE * infile;

    if ((infile = fopen(input_file.c_str(), "rb")) == NULL) {
        throw common_exception("input file was not opened");
    }
    cinfo.err = jpeg_std_error(&jerr.pub);
    try{
        //void (EncoderJPG::*func)(j_common_ptr) = &EncoderJPG::jpegErrorExit;
        jerr.pub.error_exit = jpegErrorExit;
        //cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, infile);
        (void) jpeg_read_header(&cinfo, TRUE);

        jvirt_barray_ptr *coeffs_array = jpeg_read_coefficients(&cinfo);
        jpeg_component_info* compptr_one = cinfo.comp_info;
        DCT_coefs_width = compptr_one->width_in_blocks;
        DCT_coefs_height = compptr_one->height_in_blocks;
        JBLOCKARRAY buffer_one = (cinfo.mem->access_virt_barray)((j_common_ptr)&cinfo, coeffs_array[0], 0, (JDIMENSION)1, FALSE);

    //size_t extracted_bits = 0;
        std::bitset<8> tmp;
        for (size_t i = 0; i < this->DCT_coefs_height; i++){
            for (size_t j = 0; j < this->DCT_coefs_width; j++){
                tmp = 127 + buffer_one[i][j][this->variable_elem_index];            // Работаем как с uchar
                bitset_y_components.push_back(tmp);
            }
        }

        //jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        fclose(infile);
    }
    catch (common_exception& ex){
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        throw ex;
    }
}


size_t DecoderJPG::get_header_bits_amount()
{
    //Формат Хранения хедера  //KEY##LEN@@
    // 32 - зарезервированное количество битов под размер
    // 2^8 cтепени - максимальный шаг
    // 8 * 8 - > количество битов необходимых для хранение доп. символов

    size_t amount = key_len + 4 + 6;    //Количество байтов необходимых для хранения хедера
    return amount* 8;
}


size_t DecoderJPG::get_message_bits_amount(){
    return message.size() * 8;
}



bool DecoderJPG::is_valid_capacity_without_message()
{
    size_t bits_header_amount = get_header_bits_amount();
    size_t image_capacity = DCT_coefs_height * DCT_coefs_width;
    return bits_header_amount < image_capacity;
}


bool DecoderJPG::is_valid_capacity_with_message(){
    size_t bits_header_amount = get_header_bits_amount();
    size_t bits_message_amount = get_message_bits_amount();
    size_t image_capacity = DCT_coefs_height * DCT_coefs_width;
    return (bits_header_amount + bits_message_amount) < image_capacity;
}


bool DecoderJPG::is_valid_image(){
    return (is_valid_capacity_with_message() && (header == "//Hello bro##@@"));
}


char DecoderJPG::bitset_to_char(std::bitset<8>& a){
    int res = a.to_ulong();
    res -= 127;
    return static_cast<char>(res);
}


void DecoderJPG::get_header_from_bitset(){
    if (!is_valid_capacity_without_message()){
        throw common_exception("This image does not contain the additional information");
    }

    std::bitset<8> tmp_uchar;
    std::bitset<32> len;
    size_t bit_position = 0;

    for (size_t i = 0; i < 4 + key_len; i++){
        get_char_from_bitset(tmp_uchar, bit_position, 1);
        //std::cout << tmp_uchar.to_string();
        header.push_back(static_cast<char>(tmp_uchar.to_ulong()));
        //std::cout << tmp_uchar.to_ulong() << std::endl;
    }

    get_len_message_from_bitset(len, bit_position);
    len_msg = len.to_ulong();
    for (int i = 0; i < 2; i++){
        get_char_from_bitset(tmp_uchar, bit_position, 1);
        header.push_back(static_cast<char>(tmp_uchar.to_ulong()));
    }

}


void DecoderJPG::get_message_from_bitset(){
    if (!is_valid_image()){
        throw common_exception("This image does not contain any additional message");
    }

    size_t bit_position = get_header_bits_amount();
    std::bitset<8> tmp_char;
    for (size_t  i = 0; i < len_msg; i++){
        get_char_from_bitset(tmp_char, bit_position, 1);
        message.push_back(static_cast<char>(tmp_char.to_ulong()));
    }


}

void DecoderJPG::get_char_from_bitset(std::bitset<8>& tmp_uchar, size_t& position, size_t step)
{
    for (int i = 0; i < 8; i++){
        tmp_uchar[i] = bitset_y_components[i * step  + position][0];
    }
    position += step * 8;
}


void DecoderJPG::get_len_message_from_bitset(std::bitset<32>& len_bits, size_t &position)
{
    for (int i = 0; i < 32; i++){
        len_bits[i] = bitset_y_components[i  + position][0];
    }

    position += 32;
}
