#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


#include <glm/mat3x3.hpp>


#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"
#include "SimpleEngineCore/Camera.h"


#include "SimpleEngineCore/Rendering/OpenGL/Renderer_OpenGL.h"
#include "SimpleEngineCore/Modules/UIModule.h"

namespace SimpleEngine {
    

	// constr & destr
	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: data_({width, height, title})
	{
		
		int res = Init();
        
        
       
	}
	Window::~Window() {
		Shutdown();
	}

	// public
    void Window::OnUpdate() {
        
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

	// private
	int Window::Init() {
        LOG_INFO("Creating window {0} with size {1}x{2}",
            data_.title, data_.width, data_.height);

        glfwSetErrorCallback(
            [](int error_code, const char* description) {
                LOG_CRITICAL("GLFW error: {0}", description);            
            }
        );

        if (!glfwInit()) {
            LOG_CRITICAL("Can't initialize GLFW");
            return -1;
        }

            
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(),
            nullptr, nullptr);
        
        if (!window_)
        {
            LOG_CRITICAL("Can't create window {0}", data_.title);
            return -2;
        }
       
        if (!Renderer_OpenGL::Init(window_)) {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        }

        glfwSetWindowUserPointer(window_, &data_);

        glfwSetWindowSizeCallback(window_, 
            [](GLFWwindow* window, int width, int height) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                data.width = width;
                data.height = height;

                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(window_,
            [](GLFWwindow* window, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(window_,
            [](GLFWwindow* window) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                EventWindowClose event;
                data.eventCallbackFn(event);
            }        
        );

        glfwSetFramebufferSizeCallback(window_,
            [](GLFWwindow* window, int width, int height) {
                Renderer_OpenGL::SetViewport(width, height);
            }
        );
       
        UIModule::OnWindowCreate(window_);       

        return 0;
	}

	void Window::Shutdown() {
        UIModule::OnWindowClose();
        glfwDestroyWindow(window_);
        glfwTerminate();

        
	}


}
