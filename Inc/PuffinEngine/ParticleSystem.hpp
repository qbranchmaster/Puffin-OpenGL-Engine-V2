/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_PARTICLE_SYSTEM_HPP
#define PUFFIN_PARTICLE_SYSTEM_HPP

#include <memory>
#include <string>

namespace puffin {
    class ParticleSystem {
    public:
        ParticleSystem(std::string name);

        std::string getName() const {
            return name_;
        }

    private:
        std::string name_;
    };

    using ParticleSystemPtr = std::shared_ptr<ParticleSystem>;
} // namespace puffin

#endif // PUFFIN_PARTICLE_SYSTEM_HPP