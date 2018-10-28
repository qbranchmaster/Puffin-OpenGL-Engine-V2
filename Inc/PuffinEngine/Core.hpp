#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Core {
    public:
        Core();

        ConfigurationPtr configuration() const {
            return configuration_;
        }

        InputPtr input() const {
            return input_;
        }

        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        SystemPtr system() const {
            return system_;
        }

        TimePtr time() const {
            return time_;
        }

        WindowPtr window() const {
            return window_;
        }

        void createRenderer();
        void start() const;
        void stop() const;

    private:
        ConfigurationPtr configuration_;
        InputPtr input_;
        MasterRendererPtr master_renderer_;
        SystemPtr system_;
        TimePtr time_;
        WindowPtr window_;
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP