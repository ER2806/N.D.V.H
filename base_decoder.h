#ifndef BASE_DECODER_HPP
#define BASE_DECODER_HPP
#include <string>

class base_decoder
{
public:
    base_decoder();
    virtual ~base_decoder();
    base_decoder(const std::string& in_filename);
    base_decoder(const base_decoder&) = delete;
    void set_input_file(const std::string& in_filename);
    virtual const std::string decode() = 0;
protected:
    std::string input_file;
};

#endif // BASE_DECODER_HPP
