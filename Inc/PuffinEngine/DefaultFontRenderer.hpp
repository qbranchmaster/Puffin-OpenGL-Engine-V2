/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_FONT_RENDERER_HPP
#define PUFFIN_DEFAULT_FONT_RENDERER_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>
#include <string>

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/FontRenderer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class DefaultFontRenderer : public FontRenderer {
    public:
        DefaultFontRenderer(std::string default_font);

        void render(TextPtr text);

    private:
        void loadShaders();
        void setShaderUniforms(TextPtr text);
        void createCharacterMesh();
        FT_Face createFontFace(TextPtr text);
        void renderSingleCharacter(FT_Face font_face, wchar_t character,
            TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y);
        std::vector<glm::vec3> calculateVertices(GLint cursor_x, GLint cursor_y,
            FT_BitmapGlyph bitmap);
        GLint processWhitespaces(FT_Face font_face, wchar_t character,
            TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y);
        FT_BitmapGlyph getCharacterGlyph(FT_Face font_face, wchar_t character,
            GLint outline_size);
        GLfloat calculateScreenCoordX(GLfloat x);
        GLfloat calculateScreenCoordY(GLfloat y);

        FT_Library ft_library_{};
        FT_Stroker stroker_{};
        std::unordered_map<std::string, FT_Face> font_faces_;
        std::string default_font_{};

        MeshPtr character_mesh_;
        ShaderProgramPtr default_shader_program_;
        TexturePtr character_texture_;

        enum class TextRenderType {
            NO_OUTLINE,
            OUTLINE,
            TEXT,
        };

        TextRenderType render_type_{TextRenderType::NO_OUTLINE};
    };

    using DefaultFontRendererPtr = std::shared_ptr<DefaultFontRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_FONT_RENDERER_HPP