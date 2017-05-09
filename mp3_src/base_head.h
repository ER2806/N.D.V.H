#ifndef BASE_HEAD_H
#define BASE_HEAD_H

#include <fstream>
#include <iostream>
#include "io.h"
#include "base_exception.h"

class base_header
{
public:
    virtual bool is_correct() const = 0;
    virtual bool crc_present() const = 0;
    virtual bool padding_present() const = 0;
};

#endif // BASE_HEAD_H
