#include "graphics.hpp"
#include <glfw/glfw3.h>
#include "util/win32/glextutil.h"

#include <log.hpp>

static GLFWwindow* window;

bool GraphicsInitWin32() {
    if(!glfwInit()) {
        LOG_ERR("glfwInit() failed");
        return false;
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(640, 480, "faerie", NULL, NULL);
    if(!window) {
        glfwTerminate();
        LOG_ERR("glfwCreateWindow() failed");
        return false;
    }
    // TODO Set framebuffer size callback
    glfwMakeContextCurrent(window);
    // VSync
    glfwSwapInterval(0);

    WGLEXTLoadFunctions();
    GLEXTLoadFunctions();

    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.0f, 0.2f, 1.0f);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

bool GraphicsUpdate() {
    if(glfwWindowShouldClose(window) != 0) {
        LOG("Window closed");
        return false;
    }
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glfwSwapBuffers(window);
    return true;
}

void GraphicsCleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}