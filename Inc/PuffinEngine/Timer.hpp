/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_TIMER_HPP
#define PUFFIN_TIMER_HPP

#include <GL/glew.h>

#include <chrono>
#include <functional>
#include <memory>
#include <thread>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    using TimeoutCallback = std::function<void(void)>;

    class Timer {
    public:
        Timer(const TimeoutCallback &timeout) {
            if (!timeout) {
                logError("Timer::Timer()", "Null timer function.");
                return;
            }

            timeout_func_ = timeout;
        }

        ~Timer() {
            if (running_) {
                stop();
            }
        }

        GLboolean isRunning() const {
            return running_;
        }

        void start(GLuint interval) {
            if (running_) {
                logError("Timer::start()", "Timer is already running.");
                return;
            }

            interval_ = std::chrono::milliseconds(interval);
            running_ = true;
            thread_ = std::thread(&Timer::timerThread, this);
        }

        void stop() {
            running_ = false;
            thread_.join();
        }

    private:
        void timerThread() {
            while (running_) {
                timeout_func_();
                std::this_thread::sleep_for(interval_);
            }
        }

        GLboolean running_{false};
        std::chrono::milliseconds interval_{0};
        TimeoutCallback timeout_func_{nullptr};

        std::thread thread_;
    };

    using TimerPtr = std::shared_ptr<Timer>;
} // namespace puffin

#endif // PUFFIN_TIMER_HPP