/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_INIT_CONFIG_HPP
#define PUFFIN_INIT_CONFIG_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <utility>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class InitConfig {
    public:
        static InitConfig &instance() {
            static InitConfig singleton;
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
            return static_cast<GLfloat>(frame_width_) / static_cast<GLfloat>(frame_height_);
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

        void setWaterReflectionResolution(GLuint width, GLuint height) {
            water_reflection_width_ = width;
            water_reflection_height_ = height;
        }

        std::pair<GLuint, GLuint> getWaterReflectionResolution() const {
            return std::make_pair(water_reflection_width_, water_reflection_height_);
        }

        void setWaterRefractionResolution(GLuint width, GLuint height) {
            water_refraction_width_ = width;
            water_refraction_height_ = height;
        }

        std::pair<GLuint, GLuint> getWaterRefractionResolution() const {
            return std::make_pair(water_refraction_width_, water_refraction_height_);
        }

		void setDirectionalLightShadowMapSize(GLuint size) {
            if (size == 0) {
                logError("InitConfig::setDirectionalLightShadowMapSize()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            directional_light_shadow_map_size_ = size;
        }

        GLuint getDirectionalLightShadowMapSize() const {
            return directional_light_shadow_map_size_;
        }

    private:
        InitConfig() {}
        InitConfig(const InitConfig &) = delete;
        void operator=(const InitConfig &) = delete;

        GLuint frame_width_{640};
        GLuint frame_height_{480};
        GLushort msaa_samples_{4};
        GLboolean fullscreen_{false};
        GLushort monitor_index_{0};

		GLuint directional_light_shadow_map_size_{1024};

        GLuint water_reflection_width_{640};
        GLuint water_reflection_height_{320};
        GLuint water_refraction_width_{1280};
        GLuint water_refraction_height_{720};

        std::pair<GLushort, GLushort> gl_version_{4, 0};
    };
} // namespace puffin

#endif // PUFFIN_INIT_CONFIG_HPP