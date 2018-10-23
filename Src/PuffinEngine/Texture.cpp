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

    type_ = TextureType::None;
    width_ = img_handle_.getWidth();
    height_ = img_handle_.getHeight();
    fetchChannelsCount();

    logInfo("Texture::loadImage()", "Image [" + path_ + "] loaded.");
    return true;
}