#ifndef PUFFIN_CONFIGURATION_HPP
#define PUFFIN_CONFIGURATION_HPP

#include <GL/glew.h>

#include <memory>
#include <string>
#include <vector>

#include "PuffinEngine/System.hpp"

namespace puffin {
    class Configuration {
    public:
        Configuration(SystemPtr system);

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
        SystemPtr system_;

        GLuint frame_width_{1280};
        GLuint frame_height_{720};
        GLushort msaa_samples_{4};
        GLboolean fullscreen_{false};
        GLushort monitor_index_{0};

        std::pair<GLushort, GLushort> gl_version_{4, 0};
    };

    using ConfigurationPtr = std::shared_ptr<Configuration>;
} // namespace puffin

#endif // PUFFIN_CONFIGURATION_HPP