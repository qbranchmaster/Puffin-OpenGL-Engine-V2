/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_FONT_RENDERER_HPP
#define PUFFIN_DEFAULT_FONT_RENDERER_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/FontRenderer.hpp"

namespace puffin {
	class DefaultFontRenderer : public FontRenderer {
	public:
		DefaultFontRenderer();

		void render(FrameBufferPtr frame_buffer, TextPtr text);

	private:
		FT_Library ft_library_{};
		FT_Stroker stroker_{};
		std::unordered_map<std::string, FT_Face> font_faces_;
	};

	using DefaultFontRendererPtr = std::shared_ptr<DefaultFontRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_FONT_RENDERER_HPP