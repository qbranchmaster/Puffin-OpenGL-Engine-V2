/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_EXCEPTION_HPP
#define PUFFIN_EXCEPTION_HPP

#include <exception>
#include <string>

namespace puffin {
    class Exception : public std::exception {
    public:
        Exception(std::string source, std::string message) {
            if (!message.empty()) {
                message_ = message;
            }

            if (!source.empty()) {
                source_ = source;
            }

            what_msg_ = std::string("--- [EXCEPTION] --- [" + source_ + "] " + message_);
        }

        std::string getMessage() const {
            return message_;
        }

        std::string getSource() const {
            return source_;
        }

        const char *what() const {
            return what_msg_.c_str();
        }

    private:
        std::string message_{"NO_MESSAGE"};
        std::string source_{"NO_SOURCE"};

        std::string what_msg_;
    };
} // namespace puffin

#endif // PUFFIN_EXCEPTION_HPP