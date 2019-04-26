# Puffin OpenGL Engine ver. 2.0
3D graphics rendering engine written in C++ and OpenGL API.

Previous version of this engine is no longer developed. I decided to create a new version from scratch. It am working on this project only in my free time, so it is still under development. I am trying to add new features and fix bugs as often as I can.

## Features
  - Phong and Blinn-Phong lighting,
  - Dynamic shadow mapping (directional and point lights) with PCF,
  - Fog,
  - Normal mapping,
  - Antialiasing,
  - Skybox reflections,
  - Water reflections and refractions,
  - Particle effects,
  - Postprocessing effects,
  - HDR and bloom effect,
  - Integration with IMGUI.

## Screenshots
### Screenshot #1
![Screenshot 26.04.2019](https://raw.githubusercontent.com/qbranchmaster/Puffin-OpenGL-Engine-V2/master/Screenshots/26_04_2019.png)

## Build instructions
**Required libraries:**

 - GLM ([https://glm.g-truc.net/0.9.9/index.html](https://glm.g-truc.net/0.9.9/index.html))
 - GLEW ([http://glew.sourceforge.net/](http://glew.sourceforge.net/))
 - GLFW ([https://www.glfw.org/](https://www.glfw.org/))
 - Assimp ([http://www.assimp.org/](http://www.assimp.org/))
 - FreeType ([https://www.freetype.org/](https://www.freetype.org/))
 - FreeImage ([http://freeimage.sourceforge.net/](http://freeimage.sourceforge.net/))

On Windows I recommend using Visual Studio to compile above libraries. Building instructions of these libraries can be found on library website or in source code Readme file.

After building all of specified above libraries (except GLM, which is headers files only library) there is a need to put all following files in one directory, e.g. "*binary_libs":*

 - glew32d.lib
 - glfw3.lib
 - freetype.lib
- FreeImagePlusd.lib
- FreeImaged.lib
- assimp-vc140-mt.lib

To build Puffin OpenGL Engine CMake tool is needed.
Go to Puffin Engine directory, create empty *Build* directory and then from command line execute:

    cd Build
    cmake -G "Visual Studio 15 2017" -DASSIMP_INC_PATH="C:/Libs/assimp/include" -DFREE_IMAGE_INC_PATH="C:/Libs/FreeImage/Dist/x32" -DFREE_IMAGE_WRAP_INC_PATH="C:/Libs/FreeImage/Wrapper/FreeImagePlus/dist/x32" -DFREE_TYPE_INC_PATH="C:/Libs/freetype/include" -DGLEW_INC_PATH="C:/Libs/glew/include" -DGLFW_INC_PATH="C:/Libs/glfw/include" -DGLM_INC_PATH="D:/Libs/glm" -DLIBS_BINARY_PATH="C:/Libs/binary" ..

After that go to *Build* directory and run *PuffinEngineV2.sln*. Build project and then copy following *.dll files to PuffinEngineV2.exe location (*PuffinEngineV2/Build/bin/Debug*):
 - assimp-vc140-mt.dll
 - FreeImaged.dll
 - FreeImagePlusd.dll
 - freetype.dll
 - glew32d.dll
