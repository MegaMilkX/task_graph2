#include "faerie.hpp"

#include "graphics/graphics.hpp"

#include <log.hpp>

// TODO:
// Static shit here

bool FaerieInit() {
    if(!GraphicsInitWin32()) {
        LOG_ERR("Failed to init graphics");
        return false;
    }
    // TODO:

    return true;
}

void FaerieRun() {
    LOG("Main loop start");
    do {

    } while(GraphicsUpdate());
    LOG("Main loop terminated");
}

void FaerieCleanup() {
    GraphicsCleanup();
    // TODO:
    // Cleanup
}