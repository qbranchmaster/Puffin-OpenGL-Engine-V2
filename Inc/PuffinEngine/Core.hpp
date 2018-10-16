#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include <GL/glew.h>

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/MeshManager.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Texture.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Core {
    public:
        Core();

        ConfigurationPtr configuration() const {
            return configuration_;
        }

        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        MeshManagerPtr meshManager() const {
            return mesh_manager_;
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
        MasterRendererPtr master_renderer_;
        MeshManagerPtr mesh_manager_;
        SystemPtr system_;
        TimePtr time_;
        WindowPtr window_;
    };
} // namespace puffin 

#endif // PUFFIN_CORE_HPP