#include "bmp_encoder.h"

EncoderBMP::EncoderBMP()
        : base_encoder(){
}

EncoderBMP::EncoderBMP(std::string &in_filename, std::string &out_filename, std::string &msg)
                : base_encoder(in_filename, out_filename, msg){

}


void EncoderBMP::encode(){
    if (input_file.empty()){
        throw common_exception("Input file is not indicate");
    }
    if (output_file.empty()){
        throw common_exception("Output file is not indicate");
    }
    if (message.empty())
        throw common_exception("Message is empty");

    if (!is_bmp() && !is_png())
        throw common_exception("Format must be png on bmp");

    image = QImage(input_file.c_str());
    image_height = image.height();
    image_width = image.width();
    get_vec_colors();

    if (image.isNull())
        throw common_exception("Image not foung or image was not opened");

    get_bitset_blue_colors();
    add_header_to_bitset_blue_colors();
    add_message_to_bitset_blue_colors();
    replace_new_blue_bits();
    generate_new_image();
}

// Превращает строку в массив битов
std::vector<std::bitset<8>> EncoderBMP::get_vec_bits_from_str(std::string& str){

    std::vector<std::bitset<8>> bitset_vec;
    std::bitset<8> tmp;

    for (size_t i = 0; i < str.size(); i++){
        tmp = str[i];
        bitset_vec.push_back(tmp);
    }

    return bitset_vec;
}


// Возвращает массив битов в которые запищем текст
void EncoderBMP::get_bitset_blue_colors()
{
    std::bitset<8> tmp;
    //Будем хранить по 2 бита
    size_t header_storing_bits_amount = get_header_bits_amount() / 2;
    size_t message_storing_bits_amount = message.size() * 4;
    size_t step = get_step_of_recording();
    // Header будет хранитья с 4-го пикселя
    // Шаг записи для Headera = const = 4
    size_t i = 1;
    for (; i < header_storing_bits_amount + 1; i++){
        tmp = pixels_colors[i * 4].blue();
        bitset_blue_colors.push_back(tmp);
    }

    i *= 4;
    //std::cout << "Position0 = " << i << std::endl;
    for (size_t j = 0; j < message_storing_bits_amount; j++){
        tmp = pixels_colors[i].blue();
        i += step;
        bitset_blue_colors.push_back(tmp);
    }
}


// Возвращает массив цветов изображения
void EncoderBMP::get_vec_colors()
{
    for (size_t i = 0; i < image_width; i++){
        for (size_t j = 0; j < image_height; j++){
            pixels_colors.push_back(image.pixel(i, j));
        }
    }
}


// Возвращает колчество битов которое будет неоюходимо для хранения хедера
size_t EncoderBMP::get_header_bits_amount()
{
    //Формат Хранения хедера  //KEY##LEN@@STEP@@
    // 32 - зарезервированное количество битов под размер
    // 2^8 cтепени - максимальный шаг
    // 8 * 8 - > количество битов необходимых для хранение доп. символов

    size_t amount = key.size() + 4 + 1 + 8;    //Количество байтов необходимых для хранения хедера
    return amount* 8;
}


// Возвращает шаг, с которым будем менять значение синего байта в изобраежении
size_t EncoderBMP::get_step_of_recording()
{
    size_t bits_amout = get_header_bits_amount() * 2;
    if (image_width * image_height < bits_amout){
        throw common_exception("This image does not contain the additional information");
    }
    size_t header_size = get_header_bits_amount();
    size_t image_pixels = image_width * image_height;
    size_t step = static_cast<size_t>((image_pixels - header_size * 2) / (message.size() * 4));
    if (step > 255)
        step = 255;

    else if (step < 4)
        throw common_exception("this message can't be stored in this image because  the size is so large");

    return step;
}


// Добавляет хедер в массив синих байтов
void EncoderBMP::add_header_to_bitset_blue_colors()
{
    //Формат Хранения хедера  //KEY##LEN@@STEP@@

    std::bitset<8> bits_char = '/';
    std::bitset<32> bits_int = message.size();
    //bits_char = '/';
    size_t bit_position = 0;

    //Добавим 2 знака //
    for (int i = 0; i < 2; i++)
        add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);

    //Добавление ключа
    for (size_t i = 0; i < key.size(); i++){
        bits_char = key[i];
        add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);
    }

    //Добавим 2 знака ##
    bits_char = '#';
    for (int i = 0; i < 2; i++)
        add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);

    //Добавим длину сообщение
    add_int_bitset_to_bitset_blue_colors(bits_int, bit_position);

    //Добавим 2 знака @@
    bits_char = '@';
    for (int i = 0; i < 2; i++)
        add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);

    //Добавим шаг
    bits_char = (get_step_of_recording());
    //std::cout << "size = " << bits_char.to_ulong() << std::endl;
    add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);

    //Добавим 2 знака @@
    bits_char = '@';
    for (int i = 0; i < 2; i++)
        add_char_bitset_to_bitset_blue_colors(bits_char, bit_position);

}


// Записывает сообщение в массив синих битов
void EncoderBMP::add_message_to_bitset_blue_colors(){
    if (message.empty())
        throw common_exception("Message is empty");
    size_t bit_position = get_header_bits_amount() / 2;
    //std::cout << "bit_position = " << bit_position << std::endl;
    std::bitset<8> tmp_bits_char;
    for (size_t i = 0; i < message.size(); i++){
        //std::cout << message[i] << std::endl;
        tmp_bits_char = message[i];
        add_char_bitset_to_bitset_blue_colors(tmp_bits_char, bit_position);
    }
}


// Добавляет char в массив синих байтов
void EncoderBMP::add_char_bitset_to_bitset_blue_colors(std::bitset<8> bits, size_t& index)
{
    for (int i = 0; i < 4; i++){
        //std::cout << i + index <<  " - >" << bitset_blue_colors[i + index] << " + "<< bits[2*i] << bits[2*i+1] << " = ";
        bitset_blue_colors[i + index][0] = bits[2*i];
        bitset_blue_colors[i + index][1] = bits[2*i + 1];
        //std::cout << bitset_blue_colors[i + index] << std::endl;
        //index++;
    }
    index += 4;
}


void EncoderBMP::add_int_bitset_to_bitset_blue_colors(std::bitset<32> bits, size_t& index)
{
    for (int i = 0; i < 16; i++){
        bitset_blue_colors[i + index][0] = bits[2*i];
        bitset_blue_colors[i + index][1] = bits[2*i + 1];
    }
    index += 16;
}


// Заменяем измененные биты в таблице цветов
void EncoderBMP::replace_new_blue_bits()
{
    size_t header_size = get_header_bits_amount() / 2;
    size_t message_size = message.size() * 4;
    size_t bit_position = 4;
    for (size_t i = 0; i < header_size; i++){
        pixels_colors[bit_position].setBlue(bitset_blue_colors[i].to_ulong());
        bit_position+=4;
    }
    size_t step = get_step_of_recording();
    for (size_t j = header_size; j < header_size + message_size; j++){
        pixels_colors[bit_position].setBlue(bitset_blue_colors[j].to_ulong());
        bit_position += step;
    }
}


// Создаем новое изображение с измененными пикселями
void EncoderBMP::generate_new_image(){
    QImage new_im(image_width, image_height, QImage::Format_RGB32);
    size_t cur_pix = 0;
    for (size_t i = 0; i < image_width; i++){
        for (size_t j = 0; j < image_height; j++){
            QRgb col = qRgb(pixels_colors[cur_pix].red(), pixels_colors[cur_pix].green(), pixels_colors[cur_pix].blue());
            new_im.setPixel(i, j, col);
            //new_im.setPixel(i, j, QColor(pixels_colors[cur_pix].red(), pixels_colors[cur_pix].green(), pixels_colors[cur_pix].blue()));
            cur_pix++;
        }
    }
    if (is_png()){
        QString path = toQString(output_file);
        if (!new_im.save(path,"PNG"))
            throw common_exception("Error of the creation of the file");
    }
    else{
        QString path = toQString(output_file);
        if (!new_im.save(path,"BMP"))
            throw common_exception("Error of the creation of the file");
    }
}


bool EncoderBMP::is_png(){
    if (input_file.size() < 5)
        return false;
    std::string new_str = input_file.substr(input_file.size() - 4);
    return new_str == ".png";
}


bool EncoderBMP::is_bmp(){
    if (input_file.size() < 5)
        return false;
    std::string new_str = input_file.substr(input_file.size() - 4);
    return new_str == ".bmp";
}


QString EncoderBMP::toQString(std::string& str){
    QString res;
    for (size_t i = 0; i < str.size(); i++)
        res.push_back(str[i]);
    return res;
}
