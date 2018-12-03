/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_POSTPROCESS_HPP
#define PUFFIN_POSTPROCESS_HPP

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
        friend class MasterRenderer;

    public:
        void setEffect(PostprocessEffect effect) {
            effect_ = effect;
            has_changed_ = true;
        }

        PostprocessEffect getEffect() const {
            return effect_;
        }

        void setTintColor(const glm::vec3 &color) {
            tint_color_ = color;
            has_changed_ = true;
        }

        glm::vec3 getTintColor() const {
            return tint_color_;
        }

        void setKernelSize(GLfloat size) {
            if (size <= 0.0f) {
                logError("Postprocess::setKernelSize()",
                    "Invalid postprocess kernel size.");
                return;
            }

            kernel_size_ = size;
            has_changed_ = true;
        }

        GLfloat getKernelSize() const {
            return kernel_size_;
        }

    private:
        PostprocessEffect effect_{PostprocessEffect::NONE};

        glm::vec3 tint_color_{1.0f, 1.0f, 1.0f};
        GLfloat kernel_size_{300.0f};

        GLboolean has_changed_{true};
    };

    using PostprocessPtr = std::shared_ptr<Postprocess>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_HPP