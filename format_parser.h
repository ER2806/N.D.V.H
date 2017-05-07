#ifndef FORMAT_PARSER_HPP
#define FORMAT_PARSER_HPP

#include "formats.h"
#include "stdexcept"
#include "string"

class format_parser
{
public:
    format_parser();
    format_parser(const char* filename);
    void set_filename(const char* filename);
    const enum formats parse();
    ~format_parser();
private:
    std::string filename;
};

#endif // FORMAT_PARSER_HPP
