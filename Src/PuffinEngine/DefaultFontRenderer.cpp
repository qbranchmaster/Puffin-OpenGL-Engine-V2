/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultFontRenderer.hpp"

using namespace puffin;

DefaultFontRenderer::DefaultFontRenderer(std::string default_font) {
    if (FT_Init_FreeType(&ft_library_)) {
        throw Exception("DefaultFontRenderer::DefaultFontRenderer()",
            "FreeType initialization error.");
    }

    if (FT_Stroker_New(ft_library_, &stroker_)) {
        throw Exception("DefaultFontRenderer::DefaultFontRenderer()",
            "FreeType stroker initialization error.");
    }

    if (default_font.empty()) {
        throw Exception("DefaultFontRenderer::DefaultFontRenderer()",
            "Invalid default font.");
    }

    default_font_ = default_font;

    loadShaders();
    createCharacterMesh();
}

void DefaultFontRenderer::render(TextPtr text) {
    if (!text) {
        return;
    }

    default_shader_program_->activate();

    Texture::setTextureSlot(0);
    Texture::setPixelAlignment(1);

    character_mesh_->bind();
    character_texture_->bind();

    DepthTest::instance().enable(false);
    AlphaBlend::instance().enable(true);
    AlphaBlend::instance().setBlendFunction(BlendFunction::NORMAL);

    auto font_face = createFontFace(text);
    FT_Set_Pixel_Sizes(font_face, 0, text->getFontSize());

    GLint cursor_pos_x = text->getPosition().x;
    GLint cursor_pos_y = text->getPosition().y;

    for (const auto &character : text->getText()) {
        GLint cursor_x = cursor_pos_x;
        GLint cursor_y = cursor_pos_y;

        if (text->getOutlineSize() > 0) {
            render_type_ = TextRenderType::OUTLINE;
            renderSingleCharacter(font_face, character, text, cursor_x,
                cursor_y);
            render_type_ = TextRenderType::TEXT;
        }
        else {
            render_type_ = TextRenderType::NO_OUTLINE;
        }

        renderSingleCharacter(font_face, character, text, cursor_pos_x,
            cursor_pos_y);

        if (render_type_ == TextRenderType::TEXT) {
            cursor_pos_x = cursor_x;
            cursor_pos_y = cursor_y;
        }
    }

    Texture::setPixelAlignment(4);
}

void DefaultFontRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Data/Shaders/Font.vert",
        "Data/Shaders/Font.frag");
}

void DefaultFontRenderer::setShaderUniforms(TextPtr text) {
    default_shader_program_->setUniform("color.font_texture", 0);

    if (render_type_ == TextRenderType::OUTLINE) {
        default_shader_program_->setUniform("color.font_color",
            text->getOutlineColor());
    }
    else {
        default_shader_program_->setUniform("color.font_color",
            text->getFontColor());
    }
}

void DefaultFontRenderer::createCharacterMesh() {
    character_mesh_.reset(new Mesh());
    std::vector<GLfloat> texture_coords = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    std::vector<GLuint> indices = {
        0, 1, 2, 2, 1, 3
    };

    character_mesh_->bind();
    // Index 0 for vertex positions
    character_mesh_->setMeshData(texture_coords, 1, 2);
    character_mesh_->setMeshIndices(indices);

    character_texture_.reset(new Texture());
    character_texture_->setTexture2DData(0, 0, 1, nullptr);
    character_texture_->setTextureFilter(TextureFilter::BILINEAR);
}

FT_Face DefaultFontRenderer::createFontFace(TextPtr text) {
    // Validate font
    auto font_path = text->getFont();
    if (font_path.empty()) {
        font_path = default_font_;
    }

    for (const auto &font_face : font_faces_) {
        if (font_face.first == font_path) {
            return font_face.second;
        }
    }

    if (font_path == default_font_) {
        logWarning("DefaultFontRenderer::createFontFace()",
            "Using default font.");
    }

    FT_Face font_face{};
    FT_Error result = FT_New_Face(ft_library_, font_path.c_str(), 0,
        &font_face);
    if (result != FT_Err_Ok) {
        throw Exception("DefaultFontRenderer::createFontFace()",
            "Not supported font type.");
    }

    font_faces_[font_path] = font_face;
    return font_face;
}

void DefaultFontRenderer::renderSingleCharacter(FT_Face font_face,
    wchar_t character, TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y) {
    if (processWhitespaces(font_face, character, text, cur_pos_x, cur_pos_y)) {
        return;
    }

    FT_BitmapGlyph bitmap_glyph = getCharacterGlyph(font_face, character,
        text->getOutlineSize());
    character_texture_->setTexture2DData(bitmap_glyph->bitmap.width,
        bitmap_glyph->bitmap.rows, 1, bitmap_glyph->bitmap.buffer);

    auto vertices = calculateVertices(cur_pos_x, cur_pos_y, bitmap_glyph);

    for (auto &vertex : vertices) {
        vertex.x = calculateScreenCoordX(vertex.x);
        vertex.y = calculateScreenCoordY(vertex.y);
    }

    std::vector<GLfloat> vertices_buffer;
    for (const auto &v : vertices) {
        vertices_buffer.push_back(v.x);
        vertices_buffer.push_back(v.y);
        vertices_buffer.push_back(v.z);
    }

    character_mesh_->setMeshData(vertices_buffer, 0, 3, true);
    setShaderUniforms(text);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (render_type_ == TextRenderType::NO_OUTLINE ||
        render_type_ == TextRenderType::OUTLINE) {
        cur_pos_x += (font_face->glyph->advance.x >> 6) +
            text->getHorizontalSpacing();
    }
}

std::vector<glm::vec3> DefaultFontRenderer::calculateVertices(GLint cursor_x,
    GLint cursor_y, FT_BitmapGlyph bitmap) {
    std::vector<glm::vec3> vertices(4);

    vertices[0] = glm::vec3(cursor_x + bitmap->left, cursor_y - bitmap->top,
        0.0f);
    vertices[1] = glm::vec3(vertices[0].x, vertices[0].y + bitmap->bitmap.rows,
        0.0f);
    vertices[2] = glm::vec3(vertices[0].x + bitmap->bitmap.width, vertices[0].y,
        0.0f);
    vertices[3] = glm::vec3(vertices[2].x, vertices[1].y, 0.0f);

    return vertices;
}

GLint DefaultFontRenderer::processWhitespaces(FT_Face font_face,
    wchar_t character, TextPtr text, GLint & cur_pos_x, GLint & cur_pos_y) {
    if (character == ' ') {
        if (render_type_ == TextRenderType::NO_OUTLINE ||
            render_type_ == TextRenderType::OUTLINE)
            cur_pos_x += (font_face->glyph->advance.x >> 6) +
            text->getHorizontalSpacing();

        return -1;
    }

    if (character == '\n') {
        if (render_type_ == TextRenderType::NO_OUTLINE ||
            render_type_ == TextRenderType::OUTLINE) {
            cur_pos_x = text->getPosition().x;
            cur_pos_y += text->getFontSize() + text->getVerticalSpacing();
        }

        return -1;
    }

    return 0;
}

FT_BitmapGlyph DefaultFontRenderer::getCharacterGlyph(FT_Face font_face,
    wchar_t character, GLint outline_size) {
    FT_UInt glyph_index = FT_Get_Char_Index(font_face, character);
    FT_Load_Glyph(font_face, glyph_index, FT_LOAD_DEFAULT);

    FT_Glyph glyph;
    FT_Get_Glyph(font_face->glyph, &glyph);

    if (render_type_ == TextRenderType::OUTLINE) {
        FT_Stroker_Set(stroker_, static_cast<FT_Fixed>(64 * outline_size),
            FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
        FT_Glyph_StrokeBorder(&glyph, stroker_, false, true);
    }

    if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true)) {
        throw Exception("DefaultFontRenderer::getCharacterGlyph()",
            "Cannot get character bitmap glyph.");
    }

    FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    return bitmap_glyph;
}

GLfloat DefaultFontRenderer::calculateScreenCoordX(GLfloat x) {
    static auto v_width = Configuration::instance().getFrameWidth();
    return (x / static_cast<GLfloat>(v_width) * 2.0f - 1.0f);
}

GLfloat DefaultFontRenderer::calculateScreenCoordY(GLfloat y) {
    static auto v_height = Configuration::instance().getFrameHeight();
    return (1.0f - y / static_cast<GLfloat>(v_height) * 2.0f);
}