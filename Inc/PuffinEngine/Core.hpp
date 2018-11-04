/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Core {
    public:
        Core();

        InputPtr input() const {
            return input_;
        }

        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        WindowPtr window() const {
            return window_;
        }

        void createRenderer();
        void start() const;
        void stop() const;

    private:
        InputPtr input_;
        MasterRendererPtr master_renderer_;
        WindowPtr window_;
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP