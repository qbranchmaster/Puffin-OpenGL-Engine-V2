/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CONFIGURATION_HPP
#define PUFFIN_CONFIGURATION_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <map>
#include <utility>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class Configuration {
    public:
        static Configuration& instance() {
            static Configuration singleton;
            return singleton;
        }

        void setFrameResolution(GLuint width, GLuint height);

        GLuint getFrameWidth() const {
            return frame_width_;
        }

        GLuint getFrameHeight() const {
            return frame_height_;
        }

        GLfloat getFrameAspect() const {
            return static_cast<GLfloat>(frame_width_) /
                static_cast<GLfloat>(frame_height_);
        }

        void setMsaaSamples(GLushort samples);

        GLushort getMsaaSamples() const {
            return msaa_samples_;
        }

        void setOpenGLVersion(GLushort major, GLushort minor);

        std::pair<GLushort, GLushort> getOpenGLVersion() const {
            return gl_version_;
        }

        void enableFullscreen(GLboolean enabled);

        GLboolean isFullscreenEnabled() const {
            return fullscreen_;
        }

        void setTargetMonitorIndex(GLushort index);

        GLushort getTargetMonitorIndex() const {
            return monitor_index_;
        }

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
} // namespace puffin

#endif // PUFFIN_CONFIGURATION_HPP