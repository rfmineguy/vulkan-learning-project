#include "helloTriangleApp.h"
#include "GLFW/glfw3.h"

void HelloTriangleApp::RunApp() {
    InitializeWindow();
    InitializeVulkan();
    MainLoop();
    Cleanup();
}

//Initialize 'windowHandle' and set up windowHints
void HelloTriangleApp::InitializeWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    windowHandle = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", nullptr, nullptr);
}

//Intitialize vulkan objects
void HelloTriangleApp::InitializeVulkan() {}

//Run the main loop that keps the window open and
//   processing data
void HelloTriangleApp::MainLoop() {
    while (!glfwWindowShouldClose(windowHandle)) {
        glfwPollEvents();
    }
}

//Cleans up allocated data whether it be heap, vulkan objects, glfw, etc.
void HelloTriangleApp::Cleanup() {
    glfwDestroyWindow(windowHandle);
    glfwTerminate();
}
