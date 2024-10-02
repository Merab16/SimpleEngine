#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SimpleEngineCore/Application.h"
#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"


#include <iostream>

namespace SimpleEngine {

    Application::Application() {
        LOG_INFO("Starting Application");
    }

    Application::~Application() {
        LOG_INFO("Closing Application");
    }


    int Application::Start(unsigned int window_width, unsigned int window_height, const char* title) {
        window_ = std::make_unique<Window>(window_width, window_height, title);

        while (1) {
            window_->OnUpdate();
            OnUpdate();
        }


        return 0;
    }
}