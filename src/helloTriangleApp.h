#include "vulkan/vulkan_core.h"
#include <_types/_uint32_t.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

#include <iostream>
#include <optional>

#include <fstream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

const std::vector<const char *> validationLayers = {
"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = true;
#else
const bool ENABLE_VALIDATION_LAYERS = false;
#endif
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApp {
    public:
        void RunApp();
    private:
        void InitializeWindow();
        void InitializeVulkan();
        void MainLoop();
        void Cleanup();
        void DrawFrame();

    private:
        /**
         *    @param  N/A
         *    @return N/A
         *    @purpose
         *      Creates a new vulkan instance handle for us to use
         *      Enables extensions required by the gpu
         *      Enables validation layer support
         */
        void CreateInstance();
        /**
         *    @param  N/A
         *    @return N/A
         *    @purpose
         *
         * */
        void SetupDebugMessenger();
        void CheckVkExtensionSupport();
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();

    private:
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        bool IsDeviceSuitable(VkPhysicalDevice device);

    private:
        void CreateSurface();

    private:
        void CreateSwapChain();
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& avaialablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:
        void CreateImageViews();
        void CreateGraphicsPipeline();
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void CreateRenderPass();
        void CreateFramebuffers();

    private:
        void CreateCommandPool();
        void CreateCommandBuffer();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    private:
        void CreateSyncObjects();

    private:
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
//Validation layer callback, this is where all validation layer prompts go
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                std::cerr << "Warn/Error : " << pCallbackData->pMessage << std::endl;
            }
            return VK_FALSE;
        }
        static std::vector<char> readFile(const std::string& filename) {
            std::ifstream file(filename, std::ios::ate | std::ios::binary);

            if (!file.is_open()) {
                throw std::runtime_error("failed to open file!");
            }
            size_t fileSize = file.tellg();
            std::vector<char> buffer(fileSize);
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();
            return buffer;
        }
    private:
        GLFWwindow* windowHandle;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImageView> swapChainImageViews;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;

        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

    private:
        VkSurfaceKHR surface;
};
