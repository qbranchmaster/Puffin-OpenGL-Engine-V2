/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MESH_RENDERER_HPP
#define PUFFIN_MESH_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Renderer.hpp"

namespace puffin {
	class MeshRenderer : public Renderer {
	public:
		virtual void render(FrameBufferPtr frame_buffer, MeshPtr mesh) = 0;
	};

	using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
} // namespace puffin

#endif // PUFFIN_MESH_RENDERER_HPP