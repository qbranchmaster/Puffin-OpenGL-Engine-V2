#include <iostream>

#include "EngineDemo.hpp"

using namespace puffin;

int main() {
    try {
        EngineDemo demo;
        demo.run();
    } 
    catch (const Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    std::system("pause");
    return 0;
}