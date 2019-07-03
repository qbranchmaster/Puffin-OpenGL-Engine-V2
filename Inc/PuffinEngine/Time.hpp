/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_TIME_HPP
#define PUFFIN_TIME_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <chrono>
#include <ctime>

namespace puffin {
    class Time {
        friend class MasterRenderer;

    public:
        static Time &instance() {
            static Time singleton;
            return singleton;
        }

        GLint getFpsRate() const {
            return frame_rate_;
        }

        GLdouble getDelta() const {
            return delta_;
        }

        std::string getTimeStampNowStr(char separator = ':') {
            time_t actual_time = 0;
            std::time(&actual_time);
            tm time_info = {};
#ifdef UNIX
            localtime_r(&actual_time, &time_info);
#else
            localtime_s(&time_info, &actual_time);
#endif // UNIX
            constexpr GLshort buffer_size = 32;
            GLchar time_buffer[buffer_size];
            std::string time_format =
                "%H" + std::string(&separator) + "%M" + std::string(&separator) + "%S";
            std::strftime(time_buffer, buffer_size, time_format.c_str(), &time_info);

            return std::string(time_buffer);
        }

    private:
        Time() {}
        Time(const Time &) = delete;
        void operator=(const Time &) = delete;

        void startDeltaMeasure() {
            delta_t0_ = std::chrono::system_clock::now();
        }

        void endDeltaMeasure() {
            delta_t1_ = std::chrono::system_clock::now();
            delta_ = static_cast<GLdouble>(std::chrono::duration_cast<std::chrono::microseconds>(
                         delta_t1_ - delta_t0_).count()) / 1000000.0;
        }

        void update() {
            static GLuint frames_count = 0;
            static auto prev_time = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::now();

            frames_count++;

            if (std::chrono::duration_cast<std::chrono::milliseconds>(time - prev_time).count() >=
                1000) { // 1000 ms
                prev_time = time;
                frame_rate_ = frames_count;
                frames_count = 0;
            }
        }

        std::chrono::time_point<std::chrono::system_clock> delta_t0_;
        std::chrono::time_point<std::chrono::system_clock> delta_t1_;

        GLdouble delta_{0.0};
        GLuint frame_rate_{0};
    };
} // namespace puffin

#endif // PUFFIN_TIME_HPP