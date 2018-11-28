/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_TEXTURE_HPP
#define PUFFIN_TEXTURE_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <FreeImagePlus.h>

#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    enum class TextureType {
        None,
        RawImage,
        Texture2D,
        TextureCube,
    };

    enum class TextureFilter {
        NEAREST,
        BILINEAR,
        BILINEAR_WITH_MIPMAPS,
        TRILINEAR,
    };

    enum class TextureWrap {
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
    };

    class Texture {
    public:
        Texture();
        ~Texture();

        static void setDefaultTextureFilter(TextureType type,
            TextureFilter filter);
        static void setTextureSlot(GLushort slot_index);
        static void unbindAllTextures(TextureType type);

        GLboolean loadImageRaw(std::string path);
        GLboolean loadTexture2D(std::string path, GLboolean auto_free = true);
        GLboolean loadTextureCube(std::array<std::string, 6> paths);

        void bind() const {
            if (!handle_) {
                logError("Texture::bind()", "Cannot bind null texture.");
                return;
            }

            if (type_ == TextureType::None || type_ == TextureType::RawImage) {
                logError("Texture::bind()", "Cannot bind this texture type.");
                return;
            }

            if (StateMachine::instance().bound_texture_[
                static_cast<GLushort>(type_)] == handle_) {
                return;
            }

            switch (type_) {
            case TextureType::Texture2D:
                glBindTexture(GL_TEXTURE_2D, handle_);
                break;
            case TextureType::TextureCube:
                glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
                break;
            default:
                break;
            }

            StateMachine::instance().bound_texture_[
                static_cast<GLushort>(type_)] = handle_;
        }

        void unbind() const {
            if (StateMachine::instance().bound_texture_[
                static_cast<GLushort>(type_)] == handle_) {
                switch (type_) {
                case TextureType::Texture2D:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    break;
                case TextureType::TextureCube:
                    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                    break;
                }

                StateMachine::instance().bound_texture_[
                    static_cast<GLushort>(type_)] = 0;
            }
        }

        GLushort getChannelsCount() const;
        std::string getPath() const;
        std::pair<GLuint, GLuint> getSize() const;
        TextureType getType() const;
        GLubyte* getRawData() const;

        void freeImage();
        void swapRedBlue();
        void flipVertical();
        void flipHorizontal();
        void generateMipmap();

        GLFWimage toGlfwImage() const;

        void setTextureFilter(TextureFilter filter);
        void setTextureWrap(TextureWrap wrap_mode);

    private:
        GLboolean loadImage(std::string path);
        void fetchChannelsCount();
        void setTexture2DData(void *data);

        GLuint handle_{0};
        fipImage img_handle_;
        TextureType type_{TextureType::None};
        GLboolean has_mipmap_{false};

        std::string path_;
        GLuint width_{0};
        GLuint height_{0};
        GLushort channels_{0};

        static std::map<TextureType, TextureFilter> default_texture_filter_;
        static GLushort active_slot_;
    };

    using TexturePtr = std::shared_ptr<Texture>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_HPP