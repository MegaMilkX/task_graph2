#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../task_graph/task_graph.h"

bool GraphicsInitWin32(tg::TaskGraph& tg);
bool GraphicsUpdate();
void GraphicsSwapBuffers();
void GraphicsCleanup();

#endif
