/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MATERIAL_HPP
#define PUFFIN_MATERIAL_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Material {
    public:
        void setAmbientTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setAmbientTexture()", "Null input.");
                return;
            }

            ambient_texture_ = texture;
        }

        TexturePtr getAmbientTexture() const {
            return ambient_texture_;
        }

        void setDiffuseTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setDiffuseTexture()", "Null input.");
                return;
            }

            diffuse_texture_ = texture;
        }

        TexturePtr getDiffuseTexture() const {
            return diffuse_texture_;
        }

        void setSpecularTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setSpecularTexture()", "Null input.");
                return;
            }

            specular_texture_ = texture;
        }

        TexturePtr getSpecularTexture() const {
            return specular_texture_;
        }

        void setEmissiveTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setEmissiveTexture()", "Null input.");
                return;
            }

            emissive_texture_ = texture;
        }

        TexturePtr getEmissiveTexture() const {
            return emissive_texture_;;
        }

        void setNormalMapTexture(TexturePtr texture) {
            if (!texture) {
                logError("Material::setNormalMapTexture()", "Null input.");
                return;
            }

            normalmap_texture_ = texture;
        }

        TexturePtr getNormalMapTexture() const {
            return normalmap_texture_;
        }

        void setKa(const glm::vec3 &ka) {
            ka_ = glm::vec3(
                glm::clamp(ka.r, 0.0f, 1.0f),
                glm::clamp(ka.g, 0.0f, 1.0f),
                glm::clamp(ka.b, 0.0f, 1.0f));
        }

        glm::vec3 getKa() const {
            return ka_;
        }

        void setKd(const glm::vec3 &kd) {
            kd_ = glm::vec3(
                glm::clamp(kd.r, 0.0f, 1.0f),
                glm::clamp(kd.g, 0.0f, 1.0f),
                glm::clamp(kd.b, 0.0f, 1.0f));
        }

        glm::vec3 getKd() const {
            return kd_;
        }

        void setKs(const glm::vec3 &ks) {
            ks_ = glm::vec3(
                glm::clamp(ks.r, 0.0f, 1.0f),
                glm::clamp(ks.g, 0.0f, 1.0f),
                glm::clamp(ks.b, 0.0f, 1.0f));
        }

        glm::vec3 getKs() const {
            return ks_;
        }

        void setKe(const glm::vec3 &ke) {
            ke_ = glm::vec3(
                glm::clamp(ke.r, 0.0f, 1.0f),
                glm::clamp(ke.g, 0.0f, 1.0f),
                glm::clamp(ke.b, 0.0f, 1.0f));
        }

        glm::vec3 getKe() const {
            return ke_;
        }

        void setReflectivity(GLfloat value) {
            if (value < 0.0f) {
                logError("Material::setReflectivity()", "Invalid value.");
                return;
            }

            reflectivity_ = value;
        }

        GLfloat getReflectivity() const {
            return reflectivity_;
        }

        void setShininess(GLint value) {
            shininess_ = value;
        }

        GLint getShininess() const {
            return shininess_;
        }

        void setTransparency(const glm::vec3 &transparency) {
            transparency_ = glm::vec3(
                glm::clamp(transparency.r, 0.0f, 1.0f),
                glm::clamp(transparency.g, 0.0f, 1.0f),
                glm::clamp(transparency.b, 0.0f, 1.0f));

            if (transparency_.r < 1.0f || transparency_.g < 1.0f ||
                transparency_.b < 1.0f) {
                has_transparency_ = true;
            }
            else {
                has_transparency_ = false;
            }
        }

        glm::vec3 getTransparency() const {
            return transparency_;
        }

        GLboolean hasTransparency() const {
            return has_transparency_;
        }

    private:
        GLfloat reflectivity_{0.0f};
        GLint shininess_{0};

        glm::vec3 ka_{0.0f, 0.0f, 0.0f};
        glm::vec3 kd_{0.0f, 0.0f, 0.0f};
        glm::vec3 ks_{0.0f, 0.0f, 0.0f};
        glm::vec3 ke_{0.0f, 0.0f, 0.0f};

        GLboolean has_transparency_{false};
        glm::vec3 transparency_{1.0f, 1.0f, 1.0f};

        TexturePtr ambient_texture_;
        TexturePtr diffuse_texture_;
        TexturePtr specular_texture_;
        TexturePtr normalmap_texture_;
        TexturePtr emissive_texture_;
    };

    using MaterialPtr = std::shared_ptr<Material>;
} // namespace puffin

#endif // PUFFIN_MATERIAL_HPP