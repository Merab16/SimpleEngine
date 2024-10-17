#pragma once
#include <string>
#include <functional>
#include <memory>


#include "SimpleEngineCore/Event.h"

struct GLFWwindow;

namespace SimpleEngine {

    class Window {
    public:
        using EventCallbackFn = std::function<void(BaseEvent&)>;
        
    private:
        struct WindowData {
            unsigned int width;
            unsigned int height;
            std::string title;
            EventCallbackFn eventCallbackFn;
        };
                                            
    private:
        GLFWwindow* window_ = nullptr;
        WindowData data_;

        float backgroundColor_[4]{ 0, 0.5, 0.5, 0 };
        


    private:
        int Init();
        void Shutdown();

    public:
        Window(unsigned int width, unsigned int height, const std::string& title);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        void OnUpdate();
        unsigned int GetWidth() const { return data_.width; }
        unsigned int GetHeight() const { return data_.height; }

        void SetEventCallback(const EventCallbackFn& callback) {
            data_.eventCallbackFn = callback;
        }
    };

}