#ifndef PUFFIN_SYSTEM_HPP
#define PUFFIN_SYSTEM_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace puffin {
    class System {
        friend class MasterRenderer;

    public:
        System();
        ~System();

        std::string getGpuVendor() const;
        std::string getGpuName() const;
        std::string getGlslVersion() const;

        GLushort getMonitorsCount() const;
        std::string getMonitorName(GLushort index) const;

    private:
        void initGlfw() const;
        void terminateGlfw() const;
        void initGl() const;
    };

    using SystemPtr = std::shared_ptr<System>;
} // namespace puffin

#endif // PUFFIN_SYSTEM_HPP