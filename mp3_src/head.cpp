#include "head.h"

bool header::is_correct() const {
    return correct;
}

bool header::crc_present() const {
    return crc;
}

bool header::padding_present() const {
    return padding;
}

unsigned header::frame_size() const {
    return BITS_PER_SAMPLE * bitrate / samplerate + (unsigned) padding;
}

std::istream& operator>>(std::istream& src, header& dst) {

    raw_header buffer;
    src >> buffer;

    dst.bitrate = buffer.bitrate();
    dst.samplerate = buffer.samplerate();
    dst.correct = buffer.is_correct();
    dst.crc = buffer.crc_present();
    dst.padding = buffer.padding_present();

    return src;
}

header::header(const raw_header& init) {
    correct = init.is_correct();
    crc = init.crc_present();
    bitrate = init.bitrate();
    samplerate = init.samplerate();
    padding = init.padding_present();
}
