#include "base_coder.h"

base_coder::base_coder()
{
}

base_coder::base_coder(const QString& filename) :
    input_file(filename)
{
}

base_coder::base_coder(const std::string& filename) :
    input_file(QString::fromStdString(filename))
{
}

void base_coder::set_input_file(const QString& filename)
{
    input_file = filename;
}

void base_coder::set_input_file(const std::string& filename)
{
    input_file = QString::fromStdString(filename);
}

const QString base_coder::get_input_file() const
{
    return input_file;
}

base_coder::~base_coder()
{
}

