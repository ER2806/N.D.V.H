#include "utils.h"
#include "base_exception.h"


const enum formats _get_format(const std::string& filename) {
    if (filename.length() < 4) {
        std::string message = "Incorrect filename '" + filename + "'!";
        throw std::logic_error(message);
    }
    enum formats fmt = pass;
    if (filename.length() > 4) {
        std::string last_five_letters = filename.substr(filename.length() - 5, 5);
        if (last_five_letters == ".jpeg") {
            fmt = jpg;
            return fmt;
        }
    }

    std::string last_four_letters = filename.substr(filename.length() - 4, 4);

    if (last_four_letters == ".mp3")
    {
        fmt = mp3;
    }
    if (last_four_letters == ".jpg")
    {
        fmt = jpg;
    }
    if (last_four_letters == ".bmp")
    {
        fmt = bmp;
    }
    if (last_four_letters == ".png")
    {
        fmt = png;
    }
    if (last_four_letters == ".wav")
    {
        fmt = wav;
    }
    if (fmt == pass){
        std::string message = "Unexpected format part in filename '" + filename + "'!";
        throw std::logic_error(message);
    }
    return fmt;
}


const bool _msg_to_file(const std::string& message, const std::string& file_name) {
    std::ofstream message_file(file_name, std::ios::out);
    if (!message_file) {
        std::cerr << "Can't open file '" << file_name << "'" << std::endl;
        return false;
    }
    for (unsigned i = 0; i < message.size(); ++i) {
        if (message_file.good()) {
            message_file << message[i];
        }
        else {
            std::cerr << "Incorrect content of file '" << file_name << "'" << std::endl;
            return false;
        }
    }
    return true;
}

const std::string _get_message(std::ifstream &txt_file) {
    std::string message((std::istreambuf_iterator<char>(txt_file)),
        std::istreambuf_iterator<char>());

    for (unsigned i = 0; i < message.size(); ++i) {
        if (!isprint(message[i]) && !isspace(message[i]) && !ispunct(message[i])) {
            throw common_exception("Incorrect message.");
        }
    }

    return message;
}

const bool encode(const std::string& input_file, const std::string& output_file, const std::string& msg_txt_file) {
    enum formats fmt = pass;
    try {
        fmt = _get_format(input_file);
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
        std::string message = _get_message(message_file);
    }
    catch (common_exception& err){
        std::cerr << err.what() << std::endl;
        return false;
    }

//    std::cout << "message = " << message << std::endl;

    switch (fmt) {
        case wav:
            try {
                wav_encoder coder(input_file, output_file, msg_txt_file);
                coder.encode();
                break;
            }
            catch (common_exception& err){
                std::cerr << err.what() << std::endl;
                return false;
            }
        case mp3:
            try {
                mp3_encoder coder(input_file, output_file, msg_txt_file);
                coder.encode();
                break;
            }
            catch (common_exception& err){
                std::cerr << err.what() << std::endl;
                return false;
            }

        case bmp:
            try {
                bmp_encoder coder(input_file, output_file, msg_txt_file);
                coder.encode();
                break;
            }
            catch (common_exception& err){
                std::cerr << err.what() << std::endl;
                return false;
            }
        case png:
            try {
                bmp_encoder coder(input_file, output_file, msg_txt_file);
                coder.encode();
                break;
            }
            catch (common_exception& err){
                std::cerr << err.what() << std::endl;
                return false;
            }
        case jpg:
            try { /*constructor*/
                jpg_encoder coder(input_file, output_file, msg_txt_file);
                coder.encode();
                break;
            }
            catch (common_exception& err){
                std::cerr << err.what() << std::endl;
                return false;
            }

        default:
            return false;
    }
    return false;
}

const bool decode(const std::string& input_file, const std::string& output_file) {
    enum formats fmt = pass;
    try {
        fmt = _get_format(input_file);
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
    return _msg_to_file(message, output_file);
}
