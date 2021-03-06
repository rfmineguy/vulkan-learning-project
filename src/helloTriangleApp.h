#include "vulkan/vulkan_core.h"
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <array>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

//for double buffering (or more)
// more than two can start making the cpu lag behind the gpu
const int MAX_FRAMES_IN_FLIGHT = 2;

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

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    static std::array<VkVertexInputAttributeDescription, 2> getAttribueDescription() {
        std::array<VkVertexInputAttributeDescription, 2> attributes{};
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[0].offset = offsetof(Vertex, pos);

        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[1].offset = offsetof(Vertex, color);
        return attributes;
    }
};

const std::vector<Vertex> vertices =
{{{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
 {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
 {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
 {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
0, 1, 2, 2, 3, 0
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
        void RecreateSwapChain();
        void CleanupSwapChain();
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
        void CreateCommandBuffers();
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    private:
        void CreateSyncObjects();

    private:
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags flags);

    private:
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags useFlags, VkMemoryPropertyFlags memPropFlags, VkBuffer& buffer, VkDeviceMemory& deviceMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

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
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            std::cout << "Framebuffer resized {" << width << 'x' << height << '}' << std::endl;
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
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        uint32_t currentFrame = 0;
        bool framebufferResized = false;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

    private:
        VkSurfaceKHR surface;
};
