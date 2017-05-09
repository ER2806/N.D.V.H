#ifndef RAW_HEAD_H
#define RAW_HEAD_H

#include "base_head.h"
#include "io.h"

class  raw_header : public base_header
{
public:
    unsigned bitrate() const;
    unsigned samplerate() const;
    bool is_correct() const;
    bool crc_present() const;
    bool padding_present() const;
    bool is_valid_header() const;

    friend std::istream& operator >>(std::istream&, raw_header&);
    friend std::ofstream& operator <<(std::ofstream&, raw_header&);
    void read_byte(std::ifstream&);

    const unsigned char& operator [](unsigned position) const;
    unsigned char &operator [](unsigned position);

private:
    unsigned char data[RAW_HEADER_SIZE];

};

#endif // RAW_HEAD_H
