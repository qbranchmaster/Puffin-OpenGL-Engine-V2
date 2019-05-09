#ifndef PUFFIN_PREDEFINED_MSGS_HPP
#define PUFFIN_PREDEFINED_MSGS_HPP

#include <string>

#define PUFFIN_MSG_NULL_OBJECT                                \
    "Null object."

#define PUFFIN_MSG_INVALID_VALUE                              \
    "Invalid value."

#define PUFFIN_MSG_OUT_OF_RANGE(a,b)                          \
    "Value out of range (" + std::to_string(a) + ", " + std::to_string(b) + ")."

#define PUFFIN_MSG_SHADER_UNIFORM_NOT_EXISTS(uniform_name)    \
    "Uniform [" + uniform_name + "] does not exist."

#define PUFFIN_MSG_SHADER_COMPILE_MESSAGE(shader_name,msg)    \
    "Shader file [" + shader_name + "] compile message:\n" + msg

#define PUFFIN_MSG_SHADER_COMPILE_ERROR(shader_name)          \
    "Shader file [" + shader_name + "] compile error."

#define PUFFIN_MSG_SHADER_COMPILE_SUCCESS(shader_name)        \
    "Shader file [" + shader_name + "] compile success."

#define PUFFIN_MSG_SHADER_LINK_ERROR                          \
    "Shader program link error."

#define PUFFIN_MSG_SHADER_LINK_MESSAGE(msg)                   \
    "Shader program link message:\n" + msg

#define PUFFIN_MSG_SHADER_LINK_SUCCESS                        \
    "Shader program link success."

#define PUFFIN_MSG_FILE_EMPTY_PATH                            \
    "File path is empty."

#define PUFFIN_MSG_FILE_CANNOT_OPEN(file_path)                \
    "Error loading file [" + file_path + "]."

#endif // PUFFIN_PREDEFINED_MSGS_HPP