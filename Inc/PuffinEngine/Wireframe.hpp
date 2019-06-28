/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_WIREFRAME_HPP
#define PUFFIN_WIREFRAME_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

namespace puffin {
    enum class WireframeMode {
        None,
        Overlay,
        Full,
    };

    class Wireframe {
        friend class SceneLoader;

    public:
        void setMode(WireframeMode mode) {
            mode_ = mode;
        }

        WireframeMode getMode() const {
            return mode_;
        }

        void setColor(const glm::vec3 &color) {
            color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getColor() const {
            return color_;
        }

        void setLineWidth(GLushort width) {
            if (width < 1) {
                logError("Wireframe::setLineWidth()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            line_width_ = width;

            glLineWidth(line_width_);
        }

        GLushort getLineWidth() const {
            return line_width_;
        }

        void enable(GLboolean state) {
            if (state) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

    private:
        GLboolean enabled_{false};

        WireframeMode mode_{WireframeMode::None};
        glm::vec3 color_{1.0f, 0.0f, 0.0f};
        GLushort line_width_{1};
    };

    using WireframePtr = std::shared_ptr<Wireframe>;
} // namespace puffin

#endif // PUFFIN_WIREFRAME_HPP