#include "faerie.hpp"

#include "graphics/graphics.hpp"

#include <log.hpp>

#include "task_graph/task_graph.h"

// TODO:
// Static shit here
static tg::TaskGraph taskGraph;

bool FaerieInit() {
    if(!GraphicsInitWin32(taskGraph)) {
        LOG_ERR("Failed to init graphics");
        return false;
    }
    // TODO:

    

    taskGraph.Sort();
    return true;
}

void FaerieRun() {
    LOG("Main loop start");
    while(GraphicsUpdate()) {
        taskGraph.Run();
        GraphicsSwapBuffers();
    }
    LOG("Main loop terminated");
}

void FaerieCleanup() {
    GraphicsCleanup();
    // TODO:
    // Cleanup
}