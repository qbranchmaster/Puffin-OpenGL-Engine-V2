#include "PuffinEngine/Texture.hpp"

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

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
    if (img_handle_.accessPixels()) {
        img_handle_.clear();
    }
}

std::pair<GLuint, GLuint> Texture::getSize() const {
    return std::make_pair(width_, height_);
}

TextureType Texture::getType() const {
    return type_;
}

std::string Texture::getPath() const {
    return path_;
}

GLubyte* Texture::getRawData() const {
    return img_handle_.accessPixels();
}

GLushort Texture::getChannelsCount() const {
    return channels_;
}

void Texture::fetchChannelsCount() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::fetchChannelsCount()",
            "Cannot perform this operation on empty image.");
        return;
    }

    channels_ = img_handle_.getBitsPerPixel() / 8;
    if (channels_ == 0) {
        // Image is not empty, but has less bits per pixel than 8, so in fact,
        // it has only one channel.
        channels_ = 1;
    }
}

void Texture::swapRedBlue() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::swapRedBlue()",
            "Cannot perform this operation on empty image.");
        return;
    }

    if (channels_ < 3) {
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
}

void Texture::flipVertical() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipVertical()",
            "Cannot perform this operation on empty image.");
        return;
    }

    img_handle_.flipVertical();
}

void Texture::flipHorizontal() {
    if (!img_handle_.accessPixels()) {
        logError("Texture::flipHorizontal()",
            "Cannot perform this operation on empty image.");
        return;
    }

    img_handle_.flipHorizontal();
}

GLboolean Texture::loadImage(std::string path) {
    if (path.empty()) {
        logError("Texture::loadImage()", "Empty image path.");
        return false;
    }

    path_ = path;

    if (!img_handle_.load(path_.c_str())) {
        logError("Texture::loadImage()", "Error loading image from file [" +
            path_ + "].");
        return false;
    }

    type_ = TextureType::RawImage;
    width_ = img_handle_.getWidth();
    height_ = img_handle_.getHeight();
    fetchChannelsCount();

    logInfo("Texture::loadImage()", "Image [" + path_ + "] loaded.");
    return true;
}

GLboolean Texture::loadTexture2D(std::string path) {
    if (!loadImage(path)) {
        return false;
    }

    type_ = TextureType::Texture2D;
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_BGR,
        GL_UNSIGNED_BYTE, img_handle_.accessPixels());

//    glGenerateMipmap(GL_TEXTURE_2D);
    // Default filter for opengl is linear, so it is needed to have mipmap generated
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    logInfo("Texture::loadTexture2D()", "Texture 2D [" + path + "] loaded.");

    return true;
}