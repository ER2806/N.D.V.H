#include "wav_decoder.h"
#include "string.h"
#include "fstream"
#include "bitset"
#include <QFile>
#include <QBitArray>
#include <iostream>

#define BUFSIZE 16

wav_decoder::wav_decoder()
{
}

wav_decoder::~wav_decoder()
{
}

wav_decoder::wav_decoder(const QString& in_filename)
    : base_decoder(in_filename)
{
}

wav_decoder::wav_decoder(const std::string& in_filename)
    : base_decoder(in_filename)
{
}


const QString wav_decoder::decode()
{
    unsigned int count = 0;
    std::string message;
    unsigned long len;

    FILE* in_stream = fopen(input_file.toStdString().c_str(), "rb");
    while(!feof(in_stream))
    {
        short int buff16[BUFSIZE];
        fread(buff16, 1, BUFSIZE, in_stream);

        std::bitset<8> buff;
        std::bitset<8> msg;
        if (count == 3)
        {
            buff = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                msg[i] = buff16[i];
            }
            len = msg.to_ulong();
            std::cout << "len = " << len << std::endl;
        }
        if ((count < len + 5) && (count > 4))
        {
            buff = buff16[1];
            for (int i = 0; i < 8; i++)
            {
                msg[i] = buff[i];
            }
            char letter = msg.to_ullong();
            message += letter;
        }
        count++;
    }
    return QString::fromStdString(message);
}
