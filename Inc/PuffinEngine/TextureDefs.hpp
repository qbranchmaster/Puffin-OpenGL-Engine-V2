/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_TEXTURE_DEFS_HPP
#define PUFFIN_TEXTURE_DEFS_HPP

namespace puffin {
    enum class TextureType {
        None,
        RawImage,
        Texture2D,
    };

    enum class TextureFilter {
        NEAREST,
        BILINEAR,
        BILINEAR_WITH_MIPMAPS,
        TRILINEAR,
    };
} // namespace puffin

#endif // PUFFIN_TEXTURE_DEFS_HPP