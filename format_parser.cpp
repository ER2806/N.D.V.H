#include "format_parser.h"

format_parser::format_parser()
    : filename("")
{
}

format_parser::~format_parser()
{
}


format_parser::format_parser(const char* file)
    : filename(file)
{
}


void format_parser::set_filename(const char* file)
{
    filename = file;
}


const enum formats format_parser::parse(){
    if (filename.length() < 4) {
        throw std::logic_error("Incorrect filename");
    }
    if (filename[filename.length() - 4] != '.'){
        throw std::logic_error("Incorrect filename");
    }
    std::string last_three_letters = filename.substr(filename.length() - 3, 3);

    enum formats fmt = pass;
    if (last_three_letters == "mp3")
    {
        fmt = mp3;
    }
    if (last_three_letters == "jpg")
    {
        fmt = jpg;
    }
    if (last_three_letters == "bmp")
    {
        fmt = bmp;
    }
    if (last_three_letters == "png")
    {
        fmt = png;
    }
    if (last_three_letters == "wav")
    {
        fmt = wav;
    }
    if (fmt == pass){
        throw std::logic_error("Incorrect filename");
    }
    return fmt;
}
