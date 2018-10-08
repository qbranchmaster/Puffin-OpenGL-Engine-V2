#ifndef PUFFIN_SYSTEM_HPP
#define PUFFIN_SYSTEM_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace puffin {
    class System {
    public:
        std::string getGpuVendor() const;
        std::string getGpuName() const;
        std::string getGlslVersion() const;
    };

    using SystemPtr = std::shared_ptr<System>;
} // namespace puffin

#endif // PUFFIN_SYSTEM_HPP