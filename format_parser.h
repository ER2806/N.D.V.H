#ifndef FORMAT_PARSER_HPP
#define FORMAT_PARSER_HPP

#include "formats.h"
#include "stdexcept"
#include "string"

class format_parser
{
public:
    format_parser();
    format_parser(const std::string& filename);
    void set_filename(const std::string& filename);
    const enum formats parse();
    ~format_parser();
private:
    std::string filename;
};

#endif // FORMAT_PARSER_HPP
