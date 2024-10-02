#pragma once
#include <string>

struct GLFWwindow;

namespace SimpleEngine {

    class Window {
    private:
        GLFWwindow* window_;

        unsigned int width_;
        unsigned int height_;
        std::string title_;


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
        unsigned int GetWidth() const { return width_; }
        unsigned int GetHeight() const { return height_; }

    };

}