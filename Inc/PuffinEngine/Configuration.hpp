/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CONFIGURATION_HPP
#define PUFFIN_CONFIGURATION_HPP

#include <GL/glew.h>

#include <string>
#include <vector>

namespace puffin {
    class Configuration {
    public:
        static Configuration& instance() {
            static Configuration singleton;
            return singleton;
        }

        void setFrameResolution(GLuint width, GLuint height);
        std::pair<GLuint, GLuint> getFrameResolution() const;

        void setMsaaSamples(GLushort samples);
        GLushort getMsaaSamples() const;
        std::vector<GLushort> getSupportedMsaaSamples() const;

        void setOpenGLVersion(GLushort major, GLushort minor);
        std::pair<GLushort, GLushort> getOpenGLVersion() const;

        void enableFullscreen(GLboolean enabled);
        GLboolean isFullscreenEnabled() const;

        void setTargetMonitorIndex(GLushort index);
        GLushort getTargetMonitorIndex() const;

    private:
        Configuration() {}
        Configuration(const Configuration &) = delete;
        void operator=(const Configuration &) = delete;

        GLuint frame_width_{640};
        GLuint frame_height_{480};
        GLushort msaa_samples_{4};
        GLboolean fullscreen_{false};
        GLushort monitor_index_{0};

        std::pair<GLushort, GLushort> gl_version_{4, 0};
    };

    using ConfigurationPtr = std::shared_ptr<Configuration>;
} // namespace puffin

#endif // PUFFIN_CONFIGURATION_HPP