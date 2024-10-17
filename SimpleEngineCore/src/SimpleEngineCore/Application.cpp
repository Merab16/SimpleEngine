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

        eventDispatcher_.AddEventListener<EventMouseMoved>(
            [](EventMouseMoved& event) {
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            }
        );

        eventDispatcher_.AddEventListener<EventWindowResize>(
            [](EventWindowResize& event) {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
            }
        );

        eventDispatcher_.AddEventListener<EventWindowClose>(
            [&](EventWindowClose& event) {
                LOG_INFO("[Close] Window close");
                isCloseWindow_ = true;
            }
        );

        window_->SetEventCallback(
            [&](BaseEvent& event) {
                eventDispatcher_.Dispatch(event);
            }
        );


        while (!isCloseWindow_) {
            window_->OnUpdate();
            OnUpdate();
        }

        window_ = nullptr;      // � ��������� ���������, ��� ����� �������� *GLFWwindow
                                // � ����� ���������� ����������. ����� �������???????


        return 0;
    }
}