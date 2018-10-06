#include <iostream>

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

int main() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    try {

    } 
    catch (const Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    std::system("pause");
    return 0;
}