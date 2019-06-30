/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_POSTPROCESS_HPP
#define PUFFIN_POSTPROCESS_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    enum class PostprocessEffect {
        None,
        Negative,
        Grayscale,
        Sharpen,
        Blur,
        Edge,
        Tint,
    };

    class Postprocess {
        friend class SceneLoader;

    public:
        Postprocess() {
            chromatic_aberration_texture_.reset(new Texture());
            chromatic_aberration_texture_->loadTexture2D("Data/Textures/LensAberration.bmp");
        }

        void setEffect(PostprocessEffect effect) {
            effect_ = effect;
        }

        PostprocessEffect getEffect() const {
            return effect_;
        }

        void setTintColor(const glm::vec3 &color) {
            tint_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getTintColor() const {
            return tint_color_;
        }

        void setKernelSize(GLfloat size) {
            if (size <= 0.0f) {
                logError("Postprocess::setKernelSize()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            kernel_size_ = size;
        }

        GLfloat getKernelSize() const {
            return kernel_size_;
        }

        void enableGlowBloom(GLboolean state) {
            glow_bloom_enabled_ = state;
        }

        GLboolean isGlowBloomEnabled() const {
            return glow_bloom_enabled_;
        }

        void setGlowBloomThresholdColor(const glm::vec3 &threshold) {
            glow_bloom_thresh_ = glm::vec3(glm::clamp(threshold.r, 0.0f, 1.0f),
                glm::clamp(threshold.g, 0.0f, 1.0f), glm::clamp(threshold.b, 0.0f, 1.0f));
        }

        glm::vec3 getGlowBloomThresholdColor() const {
            return glow_bloom_thresh_;
        }

        void enableDepthOfField(GLboolean state) {
            dof_enabled_ = state;
        }

        GLboolean isDepthOfFieldEnabled() const {
            return dof_enabled_;
        }

        void setDepthOfFieldMaxBlur(GLfloat max_blur) {
            if (max_blur < 0.0f || max_blur > 1.0f) {
                logError("Postprocess::setDepthOfFieldMaxBlur()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            dof_max_blur_ = max_blur;
        }

        GLfloat getDepthOfFieldMaxBlur() const {
            return dof_max_blur_;
        }

        void setAperture(GLfloat aperture) {
            if (aperture <= 0.0f) {
                logError("Postprocess::setAperture()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            aperture_ = aperture;
        }

        GLfloat getAperture() const {
            return aperture_;
        }

        void setFocusDistance(GLfloat distance) {
            if (distance <= 0.0f) {
                logError("Postprocess::setFocusDistance()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            focus_distance_ = distance;
        }

        GLfloat getFocusDistance() const {
            return focus_distance_;
        }

        void setGamma(GLfloat gamma) {
            if (gamma <= 0.0f) {
                logError("Postprocess::setGamma()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            gamma_ = gamma;
        }

        GLfloat getGamma() const {
            return gamma_;
        }

        void setExposure(GLfloat exposure) {
            if (exposure <= 0.0f) {
                logError("Postprocess::setExposure()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            exposure_ = exposure;
        }

        GLfloat getExposure() const {
            return exposure_;
        }

        void enableChromaticAberration(GLboolean state) {
            chromatic_aberration_enabled_ = state;
        }

        GLboolean isChromaticAberrationEnabled() const {
            return chromatic_aberration_enabled_;
        }

        void setChromaticAberrationLensTexture(TexturePtr texture) {
            if (!texture) {
                logError(
                    "Postprocess::setChromaticAberrationLensTexture()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            chromatic_aberration_texture_ = texture;
        }

        TexturePtr getChromaticAberrationLensTexture() const {
            return chromatic_aberration_texture_;
        }

        void setChromaticAberrationMaxChannelsOffset(const glm::vec3 &max_offsets) {
            chromatic_aberration_max_offset_ = max_offsets;
        }

        glm::vec3 getChromaticAberrationMaxChannelsOffset() const {
            return chromatic_aberration_max_offset_;
        }

    private:
        PostprocessEffect effect_{PostprocessEffect::None};

        GLboolean glow_bloom_enabled_{false};
        glm::vec3 glow_bloom_thresh_{0.5f, 0.5f, 0.5f};

        glm::vec3 tint_color_{1.0f, 1.0f, 1.0f};
        GLfloat kernel_size_{300.0f};

        GLboolean dof_enabled_{false};
        GLfloat dof_max_blur_{0.02f};

        GLfloat aperture_{0.06f};
        GLfloat focus_distance_{0.7f};

        GLfloat gamma_{2.2f};
        GLfloat exposure_{1.0f};

        GLboolean chromatic_aberration_enabled_{false};
        TexturePtr chromatic_aberration_texture_{nullptr};
        glm::vec3 chromatic_aberration_max_offset_{0.01f, 0.0f, -0.01f};
    };

    using PostprocessPtr = std::shared_ptr<Postprocess>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_HPP