#include "helloTriangleApp.h"
#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
    HelloTriangleApp app;

    try {
        app.RunApp();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
