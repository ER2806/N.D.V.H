#include "jpg_encoder.h"
#include <iostream>

void _jpeg_error_exit(j_common_ptr cinfo)
{
    char jpegLastErrorMsg[JMSG_LENGTH_MAX];
    /* Create the message */
    ( *( cinfo->err->format_message ) ) ( cinfo, jpegLastErrorMsg );

    /* Jump to the setjmp point */
    throw common_exception( jpegLastErrorMsg ); // or your preffered exception ...
}


jpg_encoder::jpg_encoder(): base_encoder(){

}

jpg_encoder::jpg_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
            : base_encoder(in_filename, out_filename, msg){
}

jpg_encoder::~jpg_encoder(){
    close_input_image();
    if (infile)
        fclose(infile);

    if (outfile)
        fclose(outfile);
}

void jpg_encoder::encode(){
    if (message.empty()){
        throw common_exception("The message is empty");
    }
    if ((infile = fopen(input_file.c_str(), "rb")) == NULL) {
        throw common_exception("input file can't open");
    }


    read_jpeg_file();
    if (!check_capacity()){
        throw common_exception("This message so large for this image");
    }
    get_bitset_y_components();
    add_header_to_bitset();
    add_message_to_bitset();
    merge_bitset_y_comp_with_image();
    create_jpeg_file();
}


void jpg_encoder::read_jpeg_file()
{
    jpegErrorManager jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);

    //void (jpg_encoder::*func)(j_common_ptr) = &jpg_encoder::jpegErrorExit;
    jerr.pub.error_exit = _jpeg_error_exit;
    //cinfo.err = jpeg_std_error(&jerr);
    try{

        jpeg_create_decompress(&cinfo);

        //throw common_exception("Incorrect image");

        jpeg_stdio_src(&cinfo, infile);
        jpeg_read_header(&cinfo, TRUE);
        coeffs_array = jpeg_read_coefficients(&cinfo);
        compptr_one = cinfo.comp_info;
        DCT_coefs_width = compptr_one->width_in_blocks;
        DCT_coefs_height = compptr_one->height_in_blocks;
        DCT_coefficients =  (cinfo.mem->access_virt_barray)((j_common_ptr)&cinfo, coeffs_array[0], 0, (JDIMENSION)1, FALSE);
        is_decompressed = true;
    }
    catch (common_exception& ex){

        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        throw ex;
    }
}


void jpg_encoder::close_input_image(){
    if (is_decompressed){
        //jpeg_finish_decompress( &(this->cinfo) );
        jpeg_destroy_decompress( &(this->cinfo) );
        fclose(infile);
        is_decompressed = false;
    }
}

void jpg_encoder::create_jpeg_file()
{
    struct jpeg_compress_struct cinfo_out;
    //struct jpeg_error_mgr jerr_out;
    jpegErrorManager jerr_out;
    cinfo_out.err = jpeg_std_error(&jerr_out.pub);
    jerr_out.pub.error_exit = _jpeg_error_exit;
    if ((this->outfile = fopen(this->output_file.c_str(), "wb")) == NULL) {
        // fprintf(stderr, "can't open %s\n", outname.c_str());
        //return 0;
        throw common_exception("input file can't open");
    }

    //cinfo_out.err = jpeg_std_error(&jerr_out.pub);
    try{
        jpeg_create_compress(&cinfo_out);
        jpeg_stdio_dest(&cinfo_out, this->outfile);

        j_compress_ptr cinfo_ptr = &cinfo_out;
        jpeg_copy_critical_parameters((j_decompress_ptr)&(this->cinfo), cinfo_ptr);
        jpeg_write_coefficients(cinfo_ptr, this->coeffs_array);

        jpeg_finish_compress( &cinfo_out );
        jpeg_destroy_compress( &cinfo_out );
        fclose(this->outfile);
    }
    catch (common_exception& ex){
        jpeg_finish_compress(&cinfo_out);
        jpeg_destroy_compress(&cinfo_out);
        fclose(infile);
        throw ex;
    }
}




void jpg_encoder::get_bitset_y_components()
{
    size_t necessary_bits = get_header_bits_amount() + get_message_bits_amount();
    size_t extracted_bits = 0;
    std::bitset<8> tmp;
    for (size_t i = 0; i < this->DCT_coefs_height; i++){
        for (size_t j = 0; j < this->DCT_coefs_width; j++){
            if (extracted_bits >= necessary_bits)
                break;
            tmp = 127 + DCT_coefficients[i][j][this->variable_elem_index];            // Working as uchar
            bitset_y_components.push_back(tmp);
            extracted_bits++;
        }
    }
}


char jpg_encoder::bitset_to_char(std::bitset<8>& a){
    int res = a.to_ulong();
    res -= 127;
    return static_cast<char>(res);
}


void jpg_encoder::merge_bitset_y_comp_with_image()
{
    size_t necessary_bits = bitset_y_components.size();
    size_t added_bits = 0;
    for (size_t i = 0; i < this->DCT_coefs_height; i++){
        for (size_t j = 0; j < this->DCT_coefs_width; j++){
            if (added_bits >= necessary_bits)
                break;
            //tmp = 127 + DCT_coefficients[i][j][this->variable_elem_index];
            //bitset_y_components.push_back(tmp);
            DCT_coefficients[i][j][this->variable_elem_index] = bitset_to_char(bitset_y_components[added_bits]);
            added_bits++;
        }
    }

}


size_t jpg_encoder::get_header_bits_amount()
{
    // Header store amount  //KEY##LEN@@
    // 32 - Reserved bits amount to size
    // 2^8 - max step
    // 8 * 8 - > bits amount for additional information

    size_t amount = key.size() + 4 + 6;    // Bytes for header
    return amount* 8;
}


size_t jpg_encoder::get_message_bits_amount(){
    return message.size() * 8;
}


bool jpg_encoder::check_capacity(){
    return (get_header_bits_amount() + get_message_bits_amount()) < (DCT_coefs_height * DCT_coefs_width);
}


void jpg_encoder::add_header_to_bitset(){
    //Header storing format  //KEY##LEN@@

    std::bitset<8> bits_char = '/';
    std::bitset<32> bits_int = message.size();
    //bits_char = '/';
    size_t bit_position = 0;

    // add //
    for (int i = 0; i < 2; i++)
        add_char_bits_to_bitset(bits_char, bit_position);

    //Добавление ключа
    for (size_t i = 0; i < key.size(); i++){
        bits_char = key[i];
        add_char_bits_to_bitset(bits_char, bit_position);
    }

    // add ##
    bits_char = '#';
    for (int i = 0; i < 2; i++)
        add_char_bits_to_bitset(bits_char, bit_position);

    // add message len
    add_int_bits_to_bitset(bits_int, bit_position);

    // add @@
    bits_char = '@';
    for (int i = 0; i < 2; i++)
        add_char_bits_to_bitset(bits_char, bit_position);

}

void jpg_encoder::add_message_to_bitset()
{
    if (message.empty())
        throw common_exception("Message is empty");
    size_t bit_position = get_header_bits_amount();
    //std::cout << "bit_position = " << bit_position << std::endl;
    std::bitset<8> tmp_bits_char;
    for (size_t i = 0; i < message.size(); i++){
        //std::cout << message[i] << std::endl;
        tmp_bits_char = message[i];
        add_char_bits_to_bitset(tmp_bits_char, bit_position);
    }
}


void jpg_encoder::add_char_bits_to_bitset(std::bitset<8>& elem, size_t& tmp_index)
{
    for (int i = 0; i < 8; i++){
        bitset_y_components[i + tmp_index][0] = elem[i];
    }

    tmp_index += 8;
}


void jpg_encoder::add_int_bits_to_bitset(std::bitset<32> &elem, size_t &tmp_index)
{
    for (int i = 0; i < 32; i++){
        bitset_y_components[i + tmp_index][0] = elem[i];
    }

    tmp_index += 32;
}
