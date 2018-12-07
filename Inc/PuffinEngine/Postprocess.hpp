/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
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

namespace puffin {
    enum class PostprocessEffect {
        NONE,
        NEGATIVE,
        GRAYSCALE,
        SHARPEN,
        BLUR,
        EDGE,
        TINT,
    };

    class Postprocess {
    public:
        void setEffect(PostprocessEffect effect) {
            effect_ = effect;
        }

        PostprocessEffect getEffect() const {
            return effect_;
        }

        void setTintColor(const glm::vec3 &color) {
            tint_color_ = glm::vec3(
                glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getTintColor() const {
            return tint_color_;
        }

        void setKernelSize(GLfloat size) {
            if (size <= 0.0f) {
                logError("Postprocess::setKernelSize()", "Invalid value.");
                return;
            }

            kernel_size_ = size;
        }

        GLfloat getKernelSize() const {
            return kernel_size_;
        }

    private:
        PostprocessEffect effect_{PostprocessEffect::NONE};

        glm::vec3 tint_color_{1.0f, 1.0f, 1.0f};
        GLfloat kernel_size_{300.0f};
    };

    using PostprocessPtr = std::shared_ptr<Postprocess>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_HPP