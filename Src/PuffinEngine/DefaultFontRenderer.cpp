/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultFontRenderer.hpp"

using namespace puffin;

DefaultFontRenderer::DefaultFontRenderer() {
	if (FT_Init_FreeType(&ft_library_)) {
		throw Exception("DefaultFontRenderer::DefaultFontRenderer()",
			"FreeType initialization error.");
	}

	if (FT_Stroker_New(ft_library_, &stroker_)) {
		throw Exception("DefaultFontRenderer::DefaultFontRenderer()",
			"FreeType stroker initialization error.");
	}
}

void DefaultFontRenderer::render(FrameBufferPtr frame_buffer, TextPtr text) {

}