#ifndef PUFFIN_TEXTURE_HPP
#define PUFFIN_TEXTURE_HPP

#include <GL/glew.h>
#include <FreeImagePlus.h>

#include <memory>
#include <string>

namespace puffin {
    enum class TextureType {
        None,
        Texture2D,
    };

    class Texture {
    public:
        Texture();
        ~Texture();

        void loadTexture2D(std::string path);

        std::string getPath() const {
            return path_;
        }

        TextureType getType() const {
            return type_;
        }

        std::pair<GLuint, GLuint> getSize() const {
            return std::make_pair(width_, height_);
        }

        unsigned char* getRawData() const {
            return img_handle_.accessPixels();
        }

    private:
        GLuint handle_{0};
        std::string path_;

        GLuint width_{0};
        GLuint height_{0};

        fipImage img_handle_;

        TextureType type_{TextureType::None};
    };

    using TexturePtr = std::shared_ptr<Texture>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_HPP