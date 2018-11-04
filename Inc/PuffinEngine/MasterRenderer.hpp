/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MASTER_RENDERER_HPP
#define PUFFIN_MASTER_RENDERER_HPP

#include <GL/glew.h>

#include <functional>
#include <memory>

#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        MasterRenderer(WindowPtr window);

        void start();
        void stop();
        void assignRenderingFunction(std::function<void()> function);

    private:
        GLboolean rendering_enabled_{false};
        std::function<void()> rendering_function_{nullptr};

        WindowPtr target_window_;
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP