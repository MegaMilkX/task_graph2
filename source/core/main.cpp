#include "faerie.hpp"

#include "../util/log.hpp"

#include <map>

#include <typeindex>

int main() {
    LOG("Faerie start...");

    FaerieInit();
    FaerieRun();
    FaerieCleanup();

    LOG("Faerie end")
    return 0;
}