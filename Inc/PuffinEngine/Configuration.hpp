/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CONFIGURATION_HPP
#define PUFFIN_CONFIGURATION_HPP

#include <GL/glew.h>

#include <map>
#include <utility>

#include "PuffinEngine/TextureDefs.hpp"

namespace puffin {
    class Configuration {
    public:
        static Configuration& instance() {
            static Configuration singleton;
            return singleton;
        }

        void setFrameResolution(GLuint width, GLuint height);
        std::pair<GLuint, GLuint> getFrameResolution() const;

        GLfloat getFrameAspect() const;

        void setMsaaSamples(GLushort samples);
        GLushort getMsaaSamples() const;

        void setOpenGLVersion(GLushort major, GLushort minor);
        std::pair<GLushort, GLushort> getOpenGLVersion() const;

        void enableFullscreen(GLboolean enabled);
        GLboolean isFullscreenEnabled() const;

        void setTargetMonitorIndex(GLushort index);
        GLushort getTargetMonitorIndex() const;

        void setDefaultTextureFilter(TextureType type, TextureFilter filter);
        TextureFilter getDefaultTextureFilter(TextureType type);

    private:
        Configuration();
        Configuration(const Configuration &) = delete;
        void operator=(const Configuration &) = delete;

        void initDefaultTextureFilter();

        GLuint frame_width_{640};
        GLuint frame_height_{480};
        GLushort msaa_samples_{4};
        GLboolean fullscreen_{false};
        GLushort monitor_index_{0};

        std::pair<GLushort, GLushort> gl_version_{4, 0};

        std::map<TextureType, TextureFilter> default_texture_filter_;
    };
} // namespace puffin

#endif // PUFFIN_CONFIGURATION_HPP