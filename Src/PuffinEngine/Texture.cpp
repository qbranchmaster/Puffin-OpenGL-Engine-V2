#include "PuffinEngine/Texture.hpp"

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Texture::Texture() {
    glGenTextures(1, &handle_);
}

Texture::~Texture() {
    if (handle_) {
        glDeleteTextures(1, &handle_);
    }
}

void Texture::loadTexture2D(std::string path) {
    if (path.empty()) {
        logError("Texture::loadTexture2D()", "Empty texture path.");
        return;
    }
    
    if (!img_handle_.load(path.c_str())) {
        logError("Texture::loadTexture2D()",
            "Error loading texture from file [" + path_ + "].");
        return;
    }

    logInfo("Texture::loadTexture2D()", "Texture file [" + path_ + "] loaded.");

    path_ = path;
    type_ = TextureType::Texture2D;
    width_ = img_handle_.getWidth();
    height_ = img_handle_.getHeight();
}