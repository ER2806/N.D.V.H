#include "wav_encoder.h"
#include "string.h"
#include "fstream"
#include "bitset"

#define BUFSIZE 16

wav_encoder::wav_encoder()
    : base_encoder()
{
}

wav_encoder::~wav_encoder()
{
}

wav_encoder::wav_encoder(const QString& in_filename, const QString& out_filename, const QString& msg)
    : base_encoder(in_filename, out_filename, msg)
{
}


wav_encoder::wav_encoder(const std::string& in_filename, const std::string& out_filename, const std::string& msg)
    : base_encoder(in_filename, out_filename, msg)
{
}

const int wav_encoder::encode()
{
    FILE* infile = fopen(input_file.toStdString().c_str(), "rb");
    FILE* outfile = fopen(output_file.toStdString().c_str(), "wb");

    unsigned long count = 0;
    std::bitset<8> msg;
    std::bitset<16> buff;
    short int buff16[BUFSIZE];
    int nb;

    std::string str_to_code = message.toStdString();
    while (!feof(infile))
    {
        nb = fread(buff16,1,BUFSIZE,infile);
        // Insert processing code here
        if (count == 3)
        {
            msg = str_to_code.length();

            buff = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                buff[i] = msg[i];
            }
            buff16[1] = buff.to_ulong();
        }
        if (count< str_to_code.length()+5 && count > 4)
        {
            msg = str_to_code[count-5];
            buff = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                buff[i] = msg[i];
            }
            buff16[1] = buff.to_ulong();
            //char c = msg.to_ullong();
        }
        fwrite(buff16,1,nb,outfile);			// Writing read data into output file
        count++;
    }
    return 0;
}
