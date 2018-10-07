#ifndef PUFFIN_CONFIGURATION_HPP
#define PUFFIN_CONFIGURATION_HPP

#include <GL/glew.h>

#include <memory>
#include <string>
#include <vector>

namespace puffin {
    class Configuration {
    public:
        void setDisplaySize(GLuint width, GLuint height);
        std::pair<GLuint, GLuint> getDisplaySize() const;

        void setDisplayFullscreen(GLboolean state);
        GLboolean getDisplayFullscreen() const;

        void setWindowCaption(std::string caption);
        std::string getWindowCaption() const;

        std::vector<GLushort> getSupportedMsaaSamples() const;
        void setMsaaSamples(GLushort samples);
        GLushort getMsaaSamples() const;

        void setOpenGLVersion(GLushort major, GLushort minor);
        std::pair<GLushort, GLushort> getOpenGLVersion() const;

    private:
        GLboolean display_fullscreen_{false};
        GLuint display_width_{1280};
        GLuint display_height_{720};
        GLushort msaa_samples_{4};

        std::string window_caption_{"Puffin Engine"};        

        std::pair<GLushort, GLushort> gl_version_{4, 0};
    };

    using ConfigurationPtr = std::shared_ptr<Configuration>;
} // namespace puffin

#endif // PUFFIN_CONFIGURATION_HPP