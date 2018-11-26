/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MATERIAL_HPP
#define PUFFIN_MATERIAL_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Material {
    public:
        void setDiffuseTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setDiffuseTexture()",
                    "Null diffuse texture.");
                return;
            }

            diffuse_texture_ = texture;
            has_changed_ = true;
        }

        TexturePtr getDiffuseTexture() const {
            return diffuse_texture_;
        }

        void setNormalMapTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setDiffuseTexture()",
                    "Null normal map texture.");
                return;
            }

            normalmap_texture_ = texture;
            has_changed_ = true;
        }

        TexturePtr getNormalMapTexture() const {
            return normalmap_texture_;
        }

        void setKa(const glm::vec3 &ka) {
            ka_ = glm::vec3(glm::clamp(ka.r, 0.0f, 1.0f),
                glm::clamp(ka.g, 0.0f, 1.0f), glm::clamp(ka.b, 0.0f, 1.0f));
            has_changed_ = true;
        }

        glm::vec3 getKa() const {
            return ka_;
        }

        void setKd(const glm::vec3 &kd) {
            kd_ = glm::vec3(glm::clamp(kd.r, 0.0f, 1.0f),
                glm::clamp(kd.g, 0.0f, 1.0f), glm::clamp(kd.b, 0.0f, 1.0f));
            has_changed_ = true;
        }

        glm::vec3 getKd() const {
            return kd_;
        }

        void setKs(const glm::vec3 &ks) {
            ks_ = glm::vec3(glm::clamp(ks.r, 0.0f, 1.0f),
                glm::clamp(ks.g, 0.0f, 1.0f), glm::clamp(ks.b, 0.0f, 1.0f));
            has_changed_ = true;
        }

        glm::vec3 getKs() const {
            return ks_;
        }

        void setReflectivity(GLfloat value) {
            if (value < 0.0f) {
                logError("Material::setReflectivity()",
                    "Invalid reflectivity value.");
                return;
            }

            reflectivity_ = value;
            has_changed_ = true;
        }

        GLfloat getReflectivity() const {
            return reflectivity_;
        }

        void setShininess(GLuint value) {
            shininess_ = value;
            has_changed_ = true;
        }

        GLuint getShininess() const {
            return shininess_;
        }

    protected:
        GLboolean has_changed_{true};

        GLfloat reflectivity_{0.0f};
        GLuint shininess_{0};

        glm::vec3 ka_{0.0f, 0.0f, 0.0f};
        glm::vec3 kd_{0.0f, 0.0f, 0.0f};
        glm::vec3 ks_{0.0f, 0.0f, 0.0f};

        TexturePtr diffuse_texture_{nullptr};
        TexturePtr normalmap_texture_{nullptr};
    };

    using MaterialPtr = std::shared_ptr<Material>;
} // namespace puffin

#endif // PUFFIN_MATERIAL_HPP