#ifndef PUFFIN_PREDEFINED_MSGS_HPP
#define PUFFIN_PREDEFINED_MSGS_HPP

#include <string>

#define PUFFIN_MSG_NULL_OBJECT               "Null object."
#define PUFFIN_MSG_OUT_OF_RANGE(a,b)         "Value out of range (" + std::to_string(a) + ", " + \
                                             std::to_string(b) + ")."
#define PUFFIN_MSG_UNIFORM_NOT_EXISTS(name)  "Uniform [" + name + "] does not exist."
#define PUFFIN_MSG_EMPTY_PATH                "File path is empty."
#define PUFFIN_MSG_FILE_CANNOT_OPEN(file)    "Error loading file [" + file + "]."

#endif // PUFFIN_PREDEFINED_MSGS_HPP
