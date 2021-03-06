/*
 * Puffin OpenGL Engine ver. 2.0.1 Demo
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "EngineDemo.hpp"

using namespace puffin;

int main() {
    try {
        EngineDemo demo;
        demo.start();
    }
    catch (const Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    std::system("pause");
    return 0;
}