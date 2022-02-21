#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <stdexcept>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

class HelloTriangleApp {
    public:
        void RunApp();
    private:
        void InitializeWindow();
        void InitializeVulkan();
        void MainLoop();
        void Cleanup();

    private:
        GLFWwindow* windowHandle;
};
