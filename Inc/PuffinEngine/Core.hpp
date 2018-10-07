#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Display.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class Core {
    public:
        Core();

        ConfigurationPtr configuration() const {
            return configuration_;
        }

        DisplayPtr display() const {
            return display_;
        }

        SystemPtr system() const {
            return system_;
        }

        void createDispaly();

    private:
        ConfigurationPtr configuration_;
        DisplayPtr display_;
        SystemPtr system_;
    };
} // namespace puffin 

#endif // PUFFIN_CORE_HPP