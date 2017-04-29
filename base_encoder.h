#ifndef BASE_ENCODER_HPP
#define BASE_ENCODER_HPP
#include <QString>
#include "base_coder.h"

class base_encoder : public base_coder
{
public:
    base_encoder();
    virtual ~base_encoder();
    base_encoder(const QString& in_filename, const QString& out_filename, const QString& msg);
    base_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg);
    void set_message(const std::string& msg);
    void set_message(const QString& msg);
    void set_output_file(const std::string& out_filename);
    void set_output_file(const QString& out_filename);
    virtual int encode() = 0;

protected:
    QString output_file;
    QString message;
};

#endif // BASE_ENCODER_HPP
