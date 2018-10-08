#ifndef PUFFIN_MASTER_RENDERER_HPP
#define PUFFIN_MASTER_RENDERER_HPP

#include <GL/glew.h>

#include <functional>
#include <memory>

#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        MasterRenderer(WindowPtr window, TimePtr time, SystemPtr system);

        void start();
        void stop();
        void assignRenderingFunction(std::function<void()> function);

    private:
        GLboolean rendering_enabled_{false};
        std::function<void()> rendering_function_{nullptr};

        SystemPtr system_;
        TimePtr time_;
        WindowPtr target_window_;
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP