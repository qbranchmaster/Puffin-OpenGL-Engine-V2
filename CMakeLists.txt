cmake_minimum_required(VERSION 2.6)

project(PuffinEngineV2)

if (MSVC)
    # Ehsc - exception handling flag
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif (MSVC)

if (UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif (UNIX)

message("CXX Flags: " ${CMAKE_CXX_FLAGS})

if (UNIX)
    add_definitions(-DUNIX)
endif (UNIX)
if (MSVC)
    add_definitions(-DWIN32)
endif (MSVC)

# Puffin Engine sources
file(GLOB PUFFIN_ENGINE_INC "Inc/PuffinEngine/*.hpp")
file(GLOB PUFFIN_ENGINE_SRC "Src/PuffinEngine/*.cpp")
file(GLOB PUFFIN_SHADERS "Shaders/*.vert"
                         "Shaders/*.frag")

source_group("PuffinEngine\\Inc" FILES ${PUFFIN_ENGINE_INC})
source_group("PuffinEngine\\Src" FILES ${PUFFIN_ENGINE_SRC})
source_group("PuffinEngine\\Shaders" FILES ${PUFFIN_SHADERS})

# Engine Demo sources
file(GLOB DEMO_SRC "Src/*.cpp")
file(GLOB DEMO_H "Inc/*.hpp")

source_group("Demo\\Src" FILES ${DEMO_SRC})
source_group("Demo\\Include" FILES ${DEMO_H})

# Includes and libraries
set(GLEW_INC_PATH "" CACHE PATH "GLEW include path")
set(GLFW_INC_PATH "" CACHE PATH "GLFW include path")
set(GLM_INC_PATH "" CACHE PATH "GLM include path")
set(FREE_IMAGE_INC_PATH "" CACHE PATH "FreeImage include path")
set(FREE_IMAGE_WRAP_INC_PATH "" CACHE PATH "FreeImage Wrapper include path")
set(ASSIMP_INC_PATH "" CACHE PATH "Assimp include path")

set(LIBS_BINARY_PATH "" CACHE PATH "Libs binaries path")

if (UNIX)
    # TODO
endif (UNIX)
if (MSVC)
    include_directories(include
                        "${CMAKE_SOURCE_DIR}/Inc"
                        ${GLEW_INC_PATH}
                        ${GLFW_INC_PATH}
                        ${GLM_INC_PATH}
                        ${FREE_IMAGE_INC_PATH}
                        ${FREE_IMAGE_WRAP_INC_PATH}
                        ${ASSIMP_INC_PATH}
                        )

    link_directories(${LIBS_BINARY_PATH})
endif (MSVC)

# Build configuration
add_executable(PuffinEngineV2 ${PUFFIN_ENGINE_INC} ${PUFFIN_ENGINE_SRC}
               ${DEMO_SRC} ${DEMO_H} ${PUFFIN_SHADERS})

if (UNIX)
    # TODO
endif (UNIX)
if (MSVC)
    target_link_libraries(PuffinEngineV2 OpenGL32.lib FreeImaged.lib
                          FreeImagePlusd.lib glew32d.lib glfw3.lib
                          assimp-vc140-mt.lib)
endif (MSVC)

set_target_properties(PuffinEngineV2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                      ${PROJECT_BINARY_DIR}/bin)

if (MSVC)
    set_target_properties(PuffinEngineV2 PROPERTIES
	                      VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif (MSVC)