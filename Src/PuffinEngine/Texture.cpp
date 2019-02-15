/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Texture.hpp"

using namespace puffin;

// Initialize default texture filter.
std::map<TextureType, TextureFilter> Texture::default_texture_filter_ = {
    {TextureType::Texture2D, TextureFilter::BILINEAR},
    {TextureType::Texture2DMultisample, TextureFilter::BILINEAR},
    {TextureType::TextureCube, TextureFilter::BILINEAR}
};

GLushort Texture::active_slot_ = 0;

Texture::Texture() {
    glGenTextures(1, &handle_);
}

Texture::~Texture() {
    freeImage();
    if (handle_) {
        glDeleteTextures(1, &handle_);
    }
}

void Texture::freeImage() {
    img_handle_.clear();
}

void Texture::setDefaultTextureFilter(TextureType type, TextureFilter filter) {
    switch (type) {
    case TextureType::Texture2D:
    case TextureType::Texture2DMultisample:
    case TextureType::TextureCube:
        switch (filter) {
        case TextureFilter::NEAREST:
        case TextureFilter::BILINEAR:
        case TextureFilter::BILINEAR_WITH_MIPMAPS:
        case TextureFilter::TRILINEAR:
            default_texture_filter_[type] = filter;
            break;
        }
        break;
    }
}

void Texture::setTextureSlot(GLushort slot_index) {
    if (slot_index != active_slot_) {
        glActiveTexture(GL_TEXTURE0 + slot_index);
        active_slot_ = slot_index;
    }
}

void Texture::unbindTextureType(TextureType type) {
    if (type == TextureType::None || type == TextureType::RawImage) {
        logError("Texture::unbindTextureType()", "Invalid texture type.");
        return;
    }

    switch (type) {
    case TextureType::Texture2D:
        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    case TextureType::Texture2DMultisample:
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        break;
    case TextureType::TextureCube:
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        break;
    }

    StateMachine::instance().bound_texture_[static_cast<GLushort>(type)] = 0;
}

GLboolean Texture::loadImage(std::string path) {
    if (path.empty()) {
        logError("Texture::loadImage()", "Invalid input.");
        return false;
    }

    if (!img_handle_.load(path.c_str())) {
        logError("Texture::loadImage()", "Loading image [" + path + "] error.");
        return false;
    }

    path_ = path;
    width_ = img_handle_.getWidth();
    height_ = img_handle_.getHeight();
    fetchChannelsCount();

    logInfo("Texture::loadImage()", "Image [" + path_ + "] loaded.");
    return true;
}

GLboolean Texture::isBound() const {
    if (StateMachine::instance().bound_texture_[static_cast<GLushort>(type_)]
        == handle_) {
        return true;
    }

    return false;
}

GLboolean Texture::loadImageRaw(std::string path) {
    if (!loadImage(path)) {
        return false;
    }

    type_ = TextureType::RawImage;

    return true;
}

GLboolean Texture::loadTexture2D(std::string path, GLboolean auto_free) {
    if (!loadImage(path)) {
        return false;
    }

    type_ = TextureType::Texture2D;

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_BGR,
        GL_UNSIGNED_BYTE, img_handle_.accessPixels());
    setTextureFilter(default_texture_filter_[TextureType::Texture2D]);
    setTextureWrap(TextureWrap::REPEAT);
    flipVertical();
    unbind();

    if (auto_free) {
        freeImage();
    }

    logInfo("Texture::loadTexture2D()", "Texture 2D [" + path + "] loaded.");
    return true;
}

GLboolean Texture::loadTextureCube(std::array<std::string, 6> paths) {
    type_ = TextureType::TextureCube;
    bind();

    for (GLushort i = 0; i < 6; i++) {
        if (!loadImage(paths[i])) {
            return false;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width_,
            height_, 0, GL_BGR, GL_UNSIGNED_BYTE, img_handle_.accessPixels());
        freeImage();

        logInfo("Texture::loadTextureCube()", "Texture [" + paths[i] +
            "] loaded.");
    }

    setTextureFilter(default_texture_filter_[TextureType::TextureCube]);
    setTextureWrap(TextureWrap::CLAMP_TO_EDGE);
    unbind();

    return true;
}

void Texture::createTextureBuffer(GLuint width, GLuint height,
    GLboolean multisample, GLboolean float_buffer) {
    type_ = multisample ? TextureType::Texture2DMultisample :
        TextureType::Texture2D;
    width_ = width;
    height_ = height;

    bind();
    setTexture2DData(nullptr, false, float_buffer);
    setTextureFilter(TextureFilter::BILINEAR);
    unbind();
}

void Texture::fetchChannelsCount() {
    channels_ = img_handle_.getBitsPerPixel() / 8;
    if (channels_ == 0) { // 4 / 8 = 0.5, so int type = 0
        // Image is not empty, but has less bits per pixel than 8, so in fact,
        // it has only one channel.
        channels_ = 1;
    }
}

void Texture::generateMipmap() {
    if (type_ != TextureType::Texture2D) {
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setTextureBorderColor(const glm::vec4 & color) {
    if (!isBound()) {
        logError("Texture::setTextureBorderColor()", "Texture is not bound.");
        return;
    }

    GLfloat border_color[] = {
        glm::clamp(color.r, 0.0f, 1.0f),
        glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f),
        glm::clamp(color.a, 0.0f, 1.0f)
    };

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

void Texture::setTextureWrap(TextureWrap wrap_mode) {
    if (!isBound()) {
        logError("Texture::setTextureWrap()", "Texture is not bound.");
        return;
    }

    switch (type_) {
    case TextureType::Texture2D:
        switch (wrap_mode) {
        case TextureWrap::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureWrap::CLAMP_TO_BORDER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_BORDER);
            break;
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            break;
        }
        break;
    case TextureType::TextureCube:
        switch (wrap_mode) {
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            break;
        default:
            logError("Texture::setTextureWrap()",
                "Not supported texture wrap.");
            break;
        }
        break;
    default:
        logError("Texture::setTextureWrap()", "Invalid texture type.");
        break;
    }
}

void Texture::setTextureFilter(TextureFilter filter) {
    if (!isBound()) {
        logError("Texture::setTextureFilter()", "Texture is not bound.");
        return;
    }

    switch (type_) {
    case TextureType::Texture2D:
        switch (filter) {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::BILINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::BILINEAR_WITH_MIPMAPS:
            generateMipmap();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::TRILINEAR:
            generateMipmap();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        break;
    case TextureType::TextureCube:
        switch (filter) {
        case TextureFilter::BILINEAR:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR);
            break;
        default:
            logError("Texture::setTextureFilter()",
                "Not supported texture filter.");
            break;
        }
        break;
    case TextureType::Texture2DMultisample:
        // TODO: FIll it.
        break;
    default:
        logError("Texture::setTextureFilter()", "Invalid texture type.");
        break;
    }
}

GLFWimage Texture::toGlfwImage() const {
    if (!img_handle_.accessPixels()) {
        logError("Texture::toGlfwImage()",
            "Cannot perform this operation on empty image.");
        return GLFWimage{};
    }

    GLFWimage img;
    img.width = width_;
    img.height = height_;
    img.pixels = getRawData();
    return img;
}

void Texture::setTexture2DData(void *data, GLboolean generate_mipmaps,
    GLboolean float_type) {
    if (!(type_ == TextureType::Texture2D ||
        type_ == TextureType::Texture2DMultisample)) {
        logError("Texture::setTexture2DData()", "Invalid texture type.");
        return;
    }

    if (!isBound()) {
        logError("Texture::setTexture2DData()", "Texture is not bound.");
        return;
    }

    if (type_ == TextureType::Texture2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, float_type ? GL_RGB16F : GL_RGB,
            width_, height_, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else if (type_ == TextureType::Texture2DMultisample) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
            Configuration::instance().getMsaaSamples(),
            float_type ? GL_RGB16F : GL_RGB, width_, height_, GL_TRUE);
    }

    // After setting new texture data there is a need to regenerate mipmaps.
    if (generate_mipmaps) {
        generateMipmap();
    }
}

void Texture::swapRedBlue() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::swapRedBlue()",
            "Cannot perform this operation on empty image.");
        return;
    }

    if (channels_ < 3 || !(type_ == TextureType::Texture2D ||
        type_ == TextureType::RawImage)) {
        logError("Texture::swapRedBlue()",
            "Cannot perform this operation on this image.");
        return;
    }

    for (GLuint y = 0; y < height_; y++) {
        auto line = img_handle_.getScanLine(y);
        for (GLuint x = 0; x < img_handle_.getLine(); x += channels_) {
            std::swap(line[x], line[x + 2]);
        }
    }

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.accessPixels());
    }
}

void Texture::flipVertical() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipVertical()",
            "Cannot perform this operation on empty image.");
        return;
    }

    if (!(type_ == TextureType::Texture2D || type_ == TextureType::RawImage)) {
        logError("Texture::flipVertical()",
            "Cannot perform this operation on this image.");
        return;
    }

    img_handle_.flipVertical();

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.accessPixels());
    }
}

void Texture::flipHorizontal() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipHorizontal()",
            "Cannot perform this operation on empty image.");
        return;
    }

    if (!(type_ == TextureType::Texture2D || type_ == TextureType::RawImage)) {
        logError("Texture::flipHorizontal()",
            "Cannot perform this operation on this image.");
        return;
    }

    img_handle_.flipHorizontal();

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.accessPixels());
    }
}