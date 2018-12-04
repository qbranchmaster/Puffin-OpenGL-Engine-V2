/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_EXCEPTION_HPP
#define PUFFIN_EXCEPTION_HPP

#include <string>

namespace puffin {
    class Exception {
    public:
        Exception(std::string source, std::string message) {
            if (!message.empty()) {
                message_ = message;
            }

            if (!source.empty()) {
                source_ = source;
            }
        }

        std::string getMessage() const {
            return message_;
        }

        std::string getSource() const {
            return source_;
        }

        std::string what() const {
            return std::string("Raised exception [" + message_ +
                "] from [" + source_ + "].");
        }

    private:
        std::string message_{"EMPTY_MESSAGE"};
        std::string source_{"UNKNOWN_SOURCE"};
    };
} // namespace puffin

#endif // PUFFIN_EXCEPTION_HPP