#ifndef BASE_CODER_HPP
#define BASE_CODER_HPP
#include <QString>

class base_coder {
public:
    base_coder();
    virtual ~base_coder();
    base_coder(const QString& filename);
    base_coder(const std::string& filename);
    void set_input_file(const QString& filename);
    void set_input_file(const std::string& filename);
    const QString get_input_file() const;
protected:
    QString input_file;
};

#endif // BASE_CODER_HPP
