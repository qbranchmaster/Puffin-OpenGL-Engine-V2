/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SYSTEM_HPP
#define PUFFIN_SYSTEM_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <utility>
#include <vector>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class System {
        friend class Core;

    public:
        static System &instance() {
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