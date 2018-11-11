/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SYSTEM_HPP
#define PUFFIN_SYSTEM_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <utility>
#include <vector>

namespace puffin {
    class System {
        friend class Core;
        friend class MasterRenderer;

    public:
        static System& instance() {
            static System singleton;
            return singleton;
        }

        std::string getGpuName() const;
        std::string getGpuVendor() const;
        std::string getGlslVersion() const;

        GLushort getMonitorsCount() const;
        std::string getMonitorName(GLushort index) const;
        std::pair<GLuint, GLuint> getMonitorSize(GLushort index) const;
        std::vector<GLushort> getSupportedMsaaSamples() const;

    private:
        System() {}
        System(const System &) = delete;
        void operator=(const System &) = delete;

        void initGlfw() const;
        void terminateGlfw() const;
        void initGl() const;
    };
} // namespace puffin

#endif // PUFFIN_SYSTEM_HPP