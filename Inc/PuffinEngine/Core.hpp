/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Core {
    public:
        Core();
        ~Core();

        CameraPtr camera() const {
            return camera_;
        }

        InputPtr input() const {
            return input_;
        }

        // TODO: Is it needed for a user? assignRenderingFunction() can
        // be also in other place.
        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        WindowPtr window() const {
            return window_;
        }

        void initialize();
        void start() const;
        void stop() const;

    private:
        CameraPtr camera_;
        InputPtr input_;
        MasterRendererPtr master_renderer_;
        WindowPtr window_;
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP