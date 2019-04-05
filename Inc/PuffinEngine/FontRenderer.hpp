/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FONT_RENDERER_HPP
#define PUFFIN_FONT_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Text.hpp"

namespace puffin {
	class FontRenderer : public Renderer {
	public:
		virtual void render(FrameBufferPtr frame_buffer, TextPtr text) = 0;
	};

	using FontRendererPtr = std::shared_ptr<FontRenderer>;
} // namespace puffin

#endif // PUFFIN_FONT_RENDERER_HPP