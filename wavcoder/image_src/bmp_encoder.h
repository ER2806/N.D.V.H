#ifndef ENCODER_BMP_H
#define ENCODER_BMP_H
/*
#include <QTGui>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>
#include "base_encoder.h"
#include "base_exception.h"


class bmp_encoder: public base_encoder
{
public:
    bmp_encoder();
    //bmp_encoder(std::string& in_filename, std::string& out_filename);
    bmp_encoder(std::string& in_filename, std::string& out_filename, std::string& msg);
    ~bmp_encoder() = default;

    void set_message(std::string& message);
    void set_message(char* message);

    virtual void encode() override;

private:
    std::string key = "Hello bro";
    QImage image;                                   //Изображение
    size_t image_width;
    size_t image_height;
    std::vector<QColor> pixels_colors;              // Массив цветов изображения
    std::vector<std::bitset<8>> bitset_blue_colors; // Синие биты цветов

    // Private methods
    std::vector<std::bitset<8>> get_vec_bits_from_str(std::string& str);
    void get_bitset_blue_colors();
    void get_vec_colors();
    size_t get_header_bits_amount();                 // Количество битов для хрения хедера
    size_t get_step_of_recording();                  // Шаг, с которым запишем биты в байты изображени
    void add_header_to_bitset_blue_colors();         // Записывает Header
    void add_message_to_bitset_blue_colors();        // Записывает текст
    void add_char_bitset_to_bitset_blue_colors(std::bitset<8>, size_t& index);   //Добавляет букву
    void add_int_bitset_to_bitset_blue_colors(std::bitset<32>, size_t& index);   // Добавляет число
    void replace_new_blue_bits();
    void generate_new_image();
    bool is_png();
    bool is_bmp();
    QString toQString(std::string& str);
};
*/
#endif // ENCODER_BMP_H
