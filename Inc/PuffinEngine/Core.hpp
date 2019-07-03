/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class Core {
    public:
        Core();
        ~Core();

        InputPtr input() const {
            return input_;
        }

        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        WindowPtr window() const {
            return window_;
        }

        void initialize();
        void start();
        void stop();

    private:
        InputPtr input_{nullptr};
        WindowPtr window_{nullptr};
        MasterRendererPtr master_renderer_{nullptr};
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP