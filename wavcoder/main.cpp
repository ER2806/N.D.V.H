#include <QCoreApplication>
#include <iostream>
#include "format_parser.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    if (argc == 3) {
        //1 аргумент - .формат, 2 аргумент - сообщение в txt
        std::string input_file = argv[1]; // format file with encoding message
        std::string output_file = argv[2]; // txt output message
        bool res = decode(input_file, output_file);
        //std::cout << "Decoding done." << std::endl;
    }
    else if (argc == 4) {
        // 1 аргумент - .формат, 2 аргумент - сообщение в txt, 3 аргумент - .формат вывода.
        std::string input_file = argv[1]; // clear format file
        std::string output_file = argv[2];
        std::string msg_txt_file = argv[3]; // format file with encoding message
        bool rslt = encode(input_file, output_file, msg_txt_file);
    }
    else {
        std::cout << "args is not valid." << std::endl;
    }
    std::cout << argc << std::endl;

    return 0;
}


/*
int main(int argc, char *argv[]){
    std::string file_in = "qq.wav";
    std::string file_out = "result.wav";
    std::string message = "qwertyuiopp[]asdfghjkll;'xcvbnm,.qweqwe";
    wav_encoder encoder(file_in, file_out, message);
    encoder.encode();
    wav_decoder decoder(file_out);
    std::string r = decoder.decode();
    std::cout << r << std::endl;

    return 0;
}
*/
