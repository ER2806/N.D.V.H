#include "utils.h"


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

    std::string message;
    switch (fmt) {
    case wav:
        try{
            wav_decoder coder(input_file);
            message = coder.decode();
        }
        catch (common_exception& err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    /*case mp3:
        try{
            mp3_decoder coder(input_file);
            message = coder.decode();
        }
        catch (err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case bmp:
        try{
            bmp_decoder coder(input_file);
            message = coder.decode();
        }
        catch (err){
            std::cerr << err.what() << std::endl;
            return false;
        }
    case png:
        try{
            png_decoder coder(input_file);
            message = coder.decode();
        }
        catch (err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    case jpg:
        try{
            jpg_decoder coder(input_file);
            message = coder.decode();
        }
        catch (err){
            std::cerr << err.what() << std::endl;
            return false;
        }
        break;
    */
    default:
        return false;
    }
    return msg_to_file(message, output_file);
}
