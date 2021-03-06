/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_TEXTURE_HPP
#define PUFFIN_TEXTURE_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <FreeImagePlus.h>

#include <glm/glm.hpp>

#include <array>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/Logger.hpp"

namespace puffin {
    enum class TextureType {
        None,
        RawImage,
        Texture2D,
        Texture2DMultisample,
        TextureCube,
    };

    enum class TextureFilter {
        Nearest,
        Bilinear,
        BilinearWithMipmaps,
        Trilinear,
    };

    enum class TextureWrap {
        Repeat,
        ClampToEdge,
        ClampToBorder,
    };

    class Texture {
    public:
        Texture();
        ~Texture();

        static void setDefaultTextureFilter(TextureType type, TextureFilter filter);
        static void setTextureSlot(GLushort slot_index);
        static void unbindTextureType(TextureType type);
        static void setPixelAlignment(GLushort aligment);

        GLboolean loadImageRaw(std::string path);
        GLboolean loadTexture2D(std::string path, GLboolean auto_free = true);
        GLboolean loadTextureCube(std::array<std::string, 6> paths);

        void bind() const {
            if (type_ == TextureType::None || type_ == TextureType::RawImage) {
                logError("Texture::bind()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
                return;
            }

            switch (type_) {
            case TextureType::Texture2D:
                glBindTexture(GL_TEXTURE_2D, handle_);
                break;
            case TextureType::Texture2DMultisample:
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, handle_);
                break;
            case TextureType::TextureCube:
                glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
                break;
            }
        }

        GLushort getChannelsCount() const {
            return channels_;
        }

        std::string getPath() const {
            return path_;
        }

        std::array<std::string, 6> getCubemapPath() const {
            return cubemap_path_;
        }

        std::pair<GLuint, GLuint> getSize() const {
            return std::make_pair(width_, height_);
        }

        TextureType getType() const {
            return type_;
        }

        GLubyte *getRawData() const {
            return img_handle_.accessPixels();
        }

        void freeImage();
        void swapRedBlue();
        void flipVertical();
        void flipHorizontal();

        GLFWimage toGlfwImage();

        void setTextureFilter(TextureFilter filter);
        void setTextureWrap(TextureWrap wrap_mode);
        void setTextureBorderColor(const glm::vec4 &color);
        void setTexture2DData(GLuint width, GLuint height, GLushort channels, void *data,
            GLboolean generate_mipmaps = true);

        GLuint getHandle() const {
            return handle_;
        }

    private:
        GLboolean loadImage(std::string path);
        void fetchChannelsCount();
        void generateMipmap();

        GLuint handle_{0};
        fipImage img_handle_;
        TextureType type_{TextureType::None};

        std::string path_;
        std::array<std::string, 6> cubemap_path_;

        GLuint width_{0};
        GLuint height_{0};
        GLushort channels_{0};

        static std::map<TextureType, TextureFilter> default_texture_filter_;
        static GLushort active_slot_;
    };

    using TexturePtr = std::shared_ptr<Texture>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_HPP