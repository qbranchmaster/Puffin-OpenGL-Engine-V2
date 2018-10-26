#ifndef PUFFIN_TEXTURE_HPP
#define PUFFIN_TEXTURE_HPP

#include <GL/glew.h>
#include <FreeImagePlus.h>

#include <memory>
#include <string>

namespace puffin {
    enum class TextureType {
        None,
        RawImage,
        Texture2D,
    };

    class Texture {
    public:
        Texture();
        ~Texture();

        GLboolean loadImage(std::string path);
        GLboolean loadTexture2D(std::string path);

        void bind() {
            switch (type_) {
            case TextureType::Texture2D:
                glBindTexture(GL_TEXTURE_2D, handle_);
                break;
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

    private:
        void fetchChannelsCount();

        GLuint handle_{0};
        fipImage img_handle_;
        TextureType type_{TextureType::None};

        std::string path_;
        GLuint width_{0};
        GLuint height_{0};
        GLushort channels_{0};
    };

    using TexturePtr = std::shared_ptr<Texture>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_HPP