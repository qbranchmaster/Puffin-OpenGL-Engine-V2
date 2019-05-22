/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_LOGGER_HPP
#define PUFFIN_LOGGER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/PredefinedMsgs.hpp"

namespace puffin {
    enum class MessageType {
        Info,
        Warning,
        Error,
    };

    class Logger {
    public:
        static Logger &instance() {
            static Logger singleton;
            return singleton;
        }

        void enable(GLboolean console_enabled, std::string file_name = "");
        void disable();

        void enableTimeStamp(GLboolean state);

        void log(std::string source, std::string message, MessageType type);

    private:
        Logger() {}

        ~Logger() {
            disable();
        }

        Logger(const Logger &) = delete;
        void operator=(const Logger &) = delete;

        std::string getHeader();
        std::string getTimeStampNow();

        GLboolean enabled_{false};
        GLboolean console_enabled_{false};
        GLboolean add_timestamp_{false};

        std::fstream log_file_;
    };

    void logInfo(std::string source, std::string message);
    void logWarning(std::string source, std::string message);
    void logError(std::string source, std::string message);
} // namespace puffin

#endif // PUFFIN_LOGGER_HPP