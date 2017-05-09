#include <QCoreApplication>
#include <iostream>
#include "format_parser.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    bool res = false;
    if (argc == 3) {
        //1 arg - decode_filename.format, 2 arg - message in text file
        std::string input_file = argv[1];
        std::string output_file = argv[2];
        res = decode(input_file, output_file);
        //std::cout << "Decoding done." << std::endl;
    }
    else if (argc == 4) {
        // 1 arg - source_filename.format, 2 arg - enocde_filename, 3 arg - message for hiding in text file.
        std::string input_file = argv[1];
        std::string output_file = argv[2];
        std::string msg_txt_file = argv[3];
        res = encode(input_file, output_file, msg_txt_file);
    }
    else {
        std::cout << "Incorrect args! Read README.md and try again." << std::endl;
    }
    if (res) {
        std::cout << "Success." << std::endl;
        return EXIT_SUCCESS;
    }
    std::cout << "Failure." << std::endl;
    return EXIT_FAILURE;
}
