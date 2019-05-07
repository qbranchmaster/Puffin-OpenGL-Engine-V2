/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_TIMER_HPP
#define PUFFIN_TIMER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <functional>
#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    using TimeoutCallback = std::function<void(void)>;

    class Timer {
        friend class MasterRenderer;

    public:
        Timer(const TimeoutCallback &timeout_func) {
            if (!timeout_func) {
                logError("Timer::Timer()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            timeout_func_ = timeout_func;
        }

        void start(GLuint interval_ms) {
            interval_ = interval_ms;
        }

        void stop() {
            interval_ = 0;
        }

    private:
        void update(GLdouble delta_time) {
            if (interval_ == 0) {
                return;
            }

            // Delta time is in [s], not [ms]
            elapsed_time_ += (delta_time * 1000.0);
            if (elapsed_time_ >= interval_) {
                timeout_func_();
                elapsed_time_ = 0;
            }
        }

        GLuint interval_{0};
        GLdouble elapsed_time_{0};

        TimeoutCallback timeout_func_{nullptr};
    };

    using TimerPtr = std::shared_ptr<Timer>;
} // namespace puffin

#endif // PUFFIN_TIMER_HPP