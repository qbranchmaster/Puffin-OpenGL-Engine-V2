/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_PREDEFINED_MSGS_HPP
#define PUFFIN_PREDEFINED_MSGS_HPP

#include <string>

namespace puffin {
    constexpr auto PUFFIN_MSG_NULL_OBJECT = "Null object.";

    constexpr auto PUFFIN_MSG_INVALID_VALUE = "Invalid value.";

    template<typename T1, typename T2>
    constexpr auto PUFFIN_MSG_OUT_OF_RANGE(T1 a, T2 b) {
        return "Value out of range (" + std::to_string(a) + ", " + std::to_string(b) + ").";
    }

    template<typename T1, typename T2>
    constexpr auto PUFFIN_MSG_SHADER_UNIFORM_NOT_EXISTS(T1 shader_name, T2 uniform_name) {
        return "Uniform [" + uniform_name + "] does not exist in shader program [" + shader_name +
            "].";
    }

    template<typename T1, typename T2>
    constexpr auto PUFFIN_MSG_SHADER_COMPILE_MESSAGE(T1 shader_file, T2 msg) {
        return "Shader file [" + shader_file + "] compile message:\n" + msg;
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_SHADER_COMPILE_ERROR(T shader_file) {
        return "Shader file [" + shader_file + "] compile error.";
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_SHADER_COMPILE_SUCCESS(T shader_file) {
        return "Shader file [" + shader_file + "] compile success.";
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_SHADER_LINK_ERROR(T shader_name) {
        return "Shader program [" + shader_name + "] link error.";
    }

    template<typename T1, typename T2>
    constexpr auto PUFFIN_MSG_SHADER_LINK_MESSAGE(T1 shader_name, T2 msg) {
        return "Shader program [" + shader_name + "] link message:\n" + msg;
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_SHADER_LINK_SUCCESS(T shader_name) {
        return "Shader program [" + shader_name + "] link success.";
    }

    constexpr auto PUFFIN_MSG_FILE_EMPTY_PATH = "File path is empty.";

    template<typename T>
    constexpr auto PUFFIN_MSG_FILE_CANNOT_OPEN(T file_path) {
        return "Error loading file [" + file_path + "].";
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_FILE_LOADED(T file_path) {
        return "File [" + file_path + "] loaded.";
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_FILE_SAVED(T file_path) {
        return "File [" + file_path + "] saved.";
    }

    template<typename T>
    constexpr auto PUFFIN_MSG_FILE_SAVE_ERROR(T file_path) {
        return "Error saving file [" + file_path + "].";
    }

    constexpr auto PUFFIN_MSG_TEXTURE_INVALID_TYPE = "Invalid texture type.";

    constexpr auto PUFFIN_MSG_TEXTURE_INVALID_FILTER = "Invalid texture filter.";

    constexpr auto PUFFIN_MSG_TEXTURE_INVALID_WRAP = "Invalid texture wrap.";

    template<typename T>
    constexpr auto PUFFIN_MSG_NAME_ALREADY_EXISTS(T name) {
        return "Name [" + name + "] already exists.";
    }
} // namespace puffin

#endif // PUFFIN_PREDEFINED_MSGS_HPP