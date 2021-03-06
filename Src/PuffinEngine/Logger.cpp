/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

void Logger::enable(GLboolean console_enabled, std::string file_name) {
    disable();

    if (!file_name.empty()) {
        log_file_.open(file_name.c_str(), std::ios::out | std::ios::trunc);
        if (!log_file_.is_open()) {
            throw Exception("Logger::enable()", "Opening file [" + file_name + "] error.");
        }

        log_file_ << getHeader();
    }

    std::cout << getHeader();

    console_enabled_ = console_enabled;
    enabled_ = true;
}

void Logger::disable() {
    if (enabled_) {
        if (log_file_.is_open()) {
            log_file_.close();
        }

        enabled_ = false;
    }
}

void Logger::enableTimeStamp(GLboolean state) {
    add_timestamp_ = state;
}

std::string Logger::getHeader() {
    constexpr std::size_t header_size = 80;
    std::string separator(header_size, '~');
    std::string header_text = "Puffin Engine Log";
    std::string header(header_size, ' ');
    header.replace(header_size / 2 - header_text.size() / 2, header_text.size(), header_text);

    return std::string(separator + "\n" + header + "\n" + separator + "\n");
}

void Logger::log(std::string source, std::string message, MessageType type) {
    if (!enabled_) {
        return;
    }

    if (source.empty()) {
        source = "NO_SOURCE";
    }

    if (message.empty()) {
        message = "NO_MESSAGE";
    }

    std::string time;
    if (add_timestamp_) {
        time = std::string(8, ' ');
        std::string stamp_str = Time::instance().getTimeStampNowStr();
        time.replace(0, stamp_str.size(), stamp_str);
        time += " | ";
    }

    std::string msg_type(10, ' ');
    switch (type) {
    case MessageType::Info:
        msg_type.replace(0, 6, "[INFO]");
        break;
    case MessageType::Warning:
        msg_type.replace(0, 9, "[WARNING]");
        break;
    case MessageType::Error:
        msg_type.replace(0, 7, "[ERROR]");
        break;
    }

    std::size_t new_line_pos = 0;
    do {
        new_line_pos = message.find('\n');
        std::string str = message.substr(0, new_line_pos);

        std::string complete_message = time + msg_type + " | " + source + ": " + str + "\n";

        if (log_file_) {
            log_file_ << complete_message << std::flush;
        }

        if (console_enabled_) {
            std::cout << complete_message << std::flush;
        }

        message = message.substr(new_line_pos + 1);
    } while (new_line_pos != std::string::npos);
}

void puffin::logInfo(std::string source, std::string message) {
    Logger::instance().log(source, message, MessageType::Info);
}

void puffin::logWarning(std::string source, std::string message) {
    Logger::instance().log(source, message, MessageType::Warning);
}

void puffin::logError(std::string source, std::string message) {
    Logger::instance().log(source, message, MessageType::Error);
}