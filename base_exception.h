#ifndef BASE_EXCEPTION_HPP
#define BASE_EXCEPTION_HPP
#include <exception>

class base_exception : public std::exception {
    protected:
        std::string message;

    public:
        explicit base_exception() = default;

        explicit base_exception(const char* message) {
            this->message = message;
        }

        explicit base_exception(std::string message) {
            this->message = message;
        }
};


#endif // BASE_EXCEPTION_HPP
