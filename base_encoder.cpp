#include "base_encoder.h"

base_encoder::base_encoder()
{
    output_file = "";
    message = "";
}


base_encoder::base_encoder(const QString& in_filename, const QString& out_filename, const QString& msg)
    : output_file(out_filename),
      message(msg),
      base_coder(in_filename)
{
}


base_encoder::base_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
    : output_file(QString::fromStdString(out_filename)),
      message(QString::fromStdString(msg)),
      base_coder(in_filename)
{
}


void base_encoder::set_message(const std::string& msg)
{
    message = QString::fromStdString(msg);
}


void base_encoder::set_message(const QString& msg)
{
    message = msg;
}


void base_encoder::set_output_file(const std::string& out_filename)
{
    output_file = QString::fromStdString(out_filename);
}


void base_encoder::set_output_file(const QString& out_filename)
{
    output_file = out_filename;
}

base_encoder::~base_encoder()
{
}
