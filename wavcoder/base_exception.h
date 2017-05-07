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


class common_exception : public base_exception {
    public:
        explicit common_exception() = default;

        explicit common_exception(const char* message)
            : base_exception(message) {
        }

        explicit common_exception(std::string message)
            : base_exception(message) {
        }

        virtual const char* what() const noexcept {
            return message.empty() ? "Something went wrong!" : message.c_str();
        }
};

#endif // BASE_EXCEPTION_HPP
