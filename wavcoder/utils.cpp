#include "utils.h"
#include "base_exception.h"


const bool msg_to_file(const std::string& message, const std::string& file_name) {
    std::ofstream message_file(file_name, std::ios::out);
    if (!message_file) {
        std::cerr << "can't open file." << std::endl;
        return false;
    }
    for (unsigned i = 0; i < message.size(); ++i) {
        if (message_file.good()) {
            message_file << message[i];
        }
        else {
            std::cerr << "file fail." << std::endl;
            return false;
        }
    }
    return true;
}

const std::string get_message(std::ifstream &txt_file) {

    std::string message((std::istreambuf_iterator<char>(txt_file)),
        std::istreambuf_iterator<char>());

    for (unsigned i = 0; i < message.size(); ++i) {
        if (!isprint(message[i]) && !isspace(message[i]) && !ispunct(message[i])) {
            throw common_exception("Wrong message.");
        }
    }

    return message;
}

const bool encode(std::string input_file, std::string output_file, std::string msg_txt_file) {
    format_parser find_format(input_file);
    enum formats fmt = pass;
    try {
        fmt = find_format.parse();
    }
    catch (std::logic_error& err) {
        std::cerr << err.what() << std::endl;
        return false;
    }

    if (input_file == output_file) {
        std::cerr << "Input and output filenames must be different." << std::endl;
        return false;
    }

    std::ifstream message_file(msg_txt_file);
    try {
        std::string message = get_message(message_file);
    }
    catch (common_exception& err){
        std::cerr << err.what() << std::endl;
        return false;
    }

//    std::cout << "message = " << message << std::endl;

    switch (fmt) {
    case wav:
        try{
            wav_encoder coder(input_file, output_file, message);
            coder.encode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case mp3:
        try{
            mp3_encoder coder(input_file, output_file, message);
            coder.encode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;

    case bmp:
        try{
            bmp_encoder coder(input_file, output_file, message);
            coder.encode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
    case png:
        try{
            bmp_encoder coder(input_file, output_file, message);
            coder.encode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case jpg:
        try{
            jpg_encoder coder(input_file, output_file, message);
            coder.encode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;

    default:
        return false;
    }

}

const bool decode(const std::string& input_file, const std::string& output_file)
{
    format_parser find_format(input_file);
    enum formats fmt = pass;
    try {
        fmt = find_format.parse();
    }
    catch (std::logic_error& err){
        std::cerr << err.what() << std::endl;
        return false;
    }

    if (input_file == output_file) {
        std::cerr << "Input and output filenames must be different." << std::endl;
        return false;
    }


    std::string message;
    switch (fmt) {
    case wav:
        try{
            wav_decoder coder(input_file);
            message = coder.decode();
            std::cout << "DECODE 1 - " << input_file << " message = " << message << std::endl;
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case mp3:
        try{
            mp3_decoder coder(input_file);
            message = coder.decode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case bmp:
        try{
            bmp_decoder coder(input_file);
            message = coder.decode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
    case png:
        try{
            bmp_decoder coder(input_file);
            message = coder.decode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case jpg:
        try{
            jpg_decoder coder(input_file);
            message = coder.decode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;

    default:
        return false;
    }
    return msg_to_file(message, output_file);
}
