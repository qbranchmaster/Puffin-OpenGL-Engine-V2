/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/Texture.hpp"

using namespace puffin;

// Initialize default texture filter
std::map<TextureType, TextureFilter> Texture::default_texture_filter_ = {
    {TextureType::Texture2D, TextureFilter::Bilinear},
    {TextureType::Texture2DMultisample, TextureFilter::Bilinear},
    {TextureType::TextureCube, TextureFilter::Bilinear}};

GLushort Texture::active_slot_ = 0;

Texture::Texture() {
    glGenTextures(1, &handle_);
}

Texture::~Texture() {
    freeImage();
    glDeleteTextures(1, &handle_);
}

void Texture::freeImage() {
    img_handle_.clear();
}

void Texture::setDefaultTextureFilter(TextureType type, TextureFilter filter) {
    switch (type) {
    case TextureType::Texture2D:
    case TextureType::Texture2DMultisample:
        switch (filter) {
        case TextureFilter::Nearest:
        case TextureFilter::Bilinear:
        case TextureFilter::BilinearWithMipmaps:
        case TextureFilter::Trilinear:
            default_texture_filter_[type] = filter;
            break;
        }
        break;
    case TextureType::TextureCube:
        switch (filter) {
        case TextureFilter::Nearest:
        case TextureFilter::Bilinear:
            default_texture_filter_[type] = filter;
            break;
        default:
            logError("Texture::setDefaultTextureFilter()", PUFFIN_MSG_TEXTURE_INVALID_FILTER);
            break;
        }
        break;
    default:
        logError("Texture::setDefaultTextureFilter()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        break;
    }
}

void Texture::setTextureSlot(GLushort slot_index) {
    if (slot_index != active_slot_) {
        glActiveTexture(GL_TEXTURE0 + slot_index);
        active_slot_ = slot_index;
    }
}

void Texture::setPixelAlignment(GLushort aligment) {
    if (aligment != 1 && aligment != 2 && aligment != 4 && aligment != 8) {
        logError("Texture::setPixelAlignment()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, aligment);
}

void Texture::unbindTextureType(TextureType type) {
    if (type == TextureType::None || type == TextureType::RawImage) {
        logError("Texture::unbindTextureType()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
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

    StateMachine::instance().bound_texture_ = 0;
}

GLboolean Texture::loadImage(std::string path) {
    if (path.empty()) {
        logError("Texture::loadImage()", PUFFIN_MSG_FILE_EMPTY_PATH);
        return false;
    }

    if (!img_handle_.load(path.c_str())) {
        logError("Texture::loadImage()", PUFFIN_MSG_FILE_CANNOT_OPEN(path));
        return false;
    }

    width_ = img_handle_.getWidth();
    height_ = img_handle_.getHeight();
    fetchChannelsCount();

    logInfo("Texture::loadImage()", PUFFIN_MSG_FILE_LOADED(path));
    return true;
}

GLboolean Texture::loadImageRaw(std::string path) {
    if (!loadImage(path)) {
        return false;
    }

    type_ = TextureType::RawImage;
    path_ = path;

    return true;
}

GLboolean Texture::loadTexture2D(std::string path, GLboolean auto_free) {
    if (!loadImage(path)) {
        return false;
    }

    type_ = TextureType::Texture2D;
    path_ = path;

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, (channels_ == 4) ? GL_RGBA : GL_RGB, width_, height_, 0,
        (channels_ == 4) ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, img_handle_.accessPixels());
    setTextureFilter(default_texture_filter_[TextureType::Texture2D]);
    setTextureWrap(TextureWrap::Repeat);
    flipVertical();

    if (auto_free) {
        freeImage();
    }

    return true;
}

GLboolean Texture::loadTextureCube(std::array<std::string, 6> paths) {
    type_ = TextureType::TextureCube;
    cubemap_path_ = paths;

    bind();

    for (GLushort i = 0; i < 6; i++) {
        GLboolean loaded = true;
        if (!loadImage(paths[i])) {
            loaded = false;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width_, height_, 0, GL_BGR,
            GL_UNSIGNED_BYTE, loaded ? img_handle_.accessPixels() : nullptr);
        freeImage();
    }

    setTextureFilter(default_texture_filter_[TextureType::TextureCube]);
    setTextureWrap(TextureWrap::ClampToEdge);

    return true;
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

void Texture::setTextureBorderColor(const glm::vec4 &color) {
    bind();

    GLfloat border_color[] = {glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f), glm::clamp(color.a, 0.0f, 1.0f)};

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

void Texture::setTextureWrap(TextureWrap wrap_mode) {
    bind();

    switch (type_) {
    case TextureType::Texture2D:
        switch (wrap_mode) {
        case TextureWrap::Repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureWrap::ClampToBorder:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
        case TextureWrap::ClampToEdge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        }
        break;
    case TextureType::TextureCube:
        switch (wrap_mode) {
        case TextureWrap::ClampToEdge:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        default:
            logError("Texture::setTextureWrap()", PUFFIN_MSG_TEXTURE_INVALID_WRAP);
            break;
        }
        break;
    default:
        logError("Texture::setTextureWrap()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        break;
    }
}

void Texture::setTextureFilter(TextureFilter filter) {
    bind();

    switch (type_) {
    case TextureType::Texture2D:
        switch (filter) {
        case TextureFilter::Nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::Bilinear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::BilinearWithMipmaps:
            generateMipmap();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::Trilinear:
            generateMipmap();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        break;
    case TextureType::TextureCube:
        switch (filter) {
        case TextureFilter::Bilinear:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        default:
            logError("Texture::setTextureFilter()", PUFFIN_MSG_TEXTURE_INVALID_FILTER);
            break;
        }
        break;
    case TextureType::Texture2DMultisample:
        // TODO: FIll it.
        break;
    default:
        logError("Texture::setTextureFilter()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        break;
    }
}

GLFWimage Texture::toGlfwImage() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::toGlfwImage()", PUFFIN_MSG_NULL_OBJECT);
        return GLFWimage{};
    }

    GLFWimage img;
    img.width = width_;
    img.height = height_;
    img.pixels = getRawData();
    return img;
}

void Texture::setTexture2DData(
    GLuint width, GLuint height, GLushort channels, void *data, GLboolean generate_mipmaps) {
    if (channels == 0 || channels > 4) {
        logError("Texture::setTexture2DData()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    type_ = TextureType::Texture2D;
    width_ = width;
    height_ = height;
    channels_ = channels;

    bind();

    GLuint internal_format = 0;
    GLuint format = 0;

    switch (channels_) {
    case 1:
        internal_format = GL_RED;
        format = GL_RED;
        break;
    case 3:
        internal_format = GL_RGB;
        format = GL_BGR;
        break;
    case 4:
        internal_format = GL_RGBA;
        format = GL_BGRA;
    }

    if (type_ == TextureType::Texture2D) {
        glTexImage2D(
            GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else if (type_ == TextureType::Texture2DMultisample) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, InitConfig::instance().getMsaaSamples(),
            GL_RGB, width_, height_, GL_TRUE);
    }

    // After setting new texture data there is a need to regenerate mipmaps
    if (generate_mipmaps) {
        generateMipmap();
    }
}

void Texture::swapRedBlue() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::swapRedBlue()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (channels_ < 3 || !(type_ == TextureType::Texture2D || type_ == TextureType::RawImage)) {
        logError("Texture::swapRedBlue()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        return;
    }

    for (GLuint y = 0; y < height_; y++) {
        auto line = img_handle_.getScanLine(y);
        for (GLuint x = 0; x < img_handle_.getLine(); x += channels_) {
            std::swap(line[x], line[x + 2]);
        }
    }

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.getWidth(), img_handle_.getHeight(), getChannelsCount(),
            img_handle_.accessPixels());
    }
}

void Texture::flipVertical() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipVertical()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (!(type_ == TextureType::Texture2D || type_ == TextureType::RawImage)) {
        logError("Texture::flipVertical()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        return;
    }

    img_handle_.flipVertical();

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.getWidth(), img_handle_.getHeight(), getChannelsCount(),
            img_handle_.accessPixels());
    }
}

void Texture::flipHorizontal() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipHorizontal()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (!(type_ == TextureType::Texture2D || type_ == TextureType::RawImage)) {
        logError("Texture::flipHorizontal()", PUFFIN_MSG_TEXTURE_INVALID_TYPE);
        return;
    }

    img_handle_.flipHorizontal();

    if (type_ == TextureType::Texture2D) {
        setTexture2DData(img_handle_.getWidth(), img_handle_.getHeight(), getChannelsCount(),
            img_handle_.accessPixels());
    }
}