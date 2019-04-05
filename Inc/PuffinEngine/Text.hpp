/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_TEXT_HPP
#define PUFFIN_TEXT_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

namespace puffin {
	class Text {
	public:
		Text(std::wstring text, const glm::uvec2 &position,
			GLuint font_size, std::string font = "") {
			setText(text);
			setPosition(position.x, position.y);
			setFontSize(font_size);
			setFont(font);
		}

		void setPosition(GLint x, GLint y) {
			position_.x = x;
			position_.y = y;
		}

		glm::ivec2 getPosition() const {
			return position_;
		}

		void setVerticalSpacing(GLint spacing) {
			vertical_spacing_ = spacing;
		}

		GLint getVerticalSpacing() const {
			return vertical_spacing_;
		}

		void setHorizontalSpacing(GLint spacing) {
			horizontal_spacing_ = spacing;
		}

		GLint getHorizontalSpacing() const {
			return horizontal_spacing_;
		}

		void setOutlineColor(const glm::vec3 &color) {
			outline_color_ = glm::vec3(
				glm::clamp(color.r, 0.0f, 1.0f),
				glm::clamp(color.g, 0.0f, 1.0f),
				glm::clamp(color.b, 0.0f, 1.0f));
		}

		glm::vec3 getOutlineColor() const {
			return outline_color_;
		}

		void setOutlineSize(GLushort size) {
			outline_size_ = size;
		}

		GLushort getOutlineSize() const {
			return outline_size_;
		}

		void setFontColor(const glm::vec3 &color) {
			font_color_ = glm::vec3(
				glm::clamp(color.r, 0.0f, 1.0f),
				glm::clamp(color.g, 0.0f, 1.0f),
				glm::clamp(color.b, 0.0f, 1.0f));
		}

		glm::vec3 getFontColor() const {
			return font_color_;
		}

		void setFontSize(GLuint font_size) {
			font_size_ = font_size;
		}

		GLuint getFontSize() const {
			return font_size_;
		}

		void setFont(std::string font) {
			font_ = font;
		}

		std::string getFont() const {
			return font_;
		}

		void setText(std::wstring text) {
			text_ = text;
		}

		std::wstring getText() const {
			return text_;
		}

	private:
		std::wstring text_{L""};
		std::string font_{""};

		glm::ivec2 position_{0, 0};
		GLint vertical_spacing_{0};
		GLint horizontal_spacing_{0};

		GLuint font_size_{16};
		glm::vec3 font_color_{1.0f, 1.0f, 1.0f};

		GLushort outline_size_{0};
		glm::vec3 outline_color_{0.0f, 0.0f, 0.0f};
	};

	using TextPtr = std::shared_ptr<Text>;
} // namespace puffin

#endif // PUFFIN_TEXT_HPP