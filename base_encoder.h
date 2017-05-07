#ifndef BASE_ENCODER_HPP
#define BASE_ENCODER_HPP
#include <string>

class base_encoder
{
public:
    base_encoder();
    virtual ~base_encoder();
    base_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg);
    void set_message(const std::string& msg);
    void set_output_file(const std::string& out_filename);
    void set_input_file(const std::string& in_filename);
    virtual const bool encode() = 0;

protected:
    std::string input_file;
    std::string output_file;
    std::string message;
};

#endif // BASE_ENCODER_HPP
