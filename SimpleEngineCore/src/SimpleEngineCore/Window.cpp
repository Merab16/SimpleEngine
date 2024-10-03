#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"


namespace SimpleEngine {
    static bool isGLFWinitialized = false;

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
        glClearColor(0, 0.5, 0.5, 0);  
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

	// private
	int Window::Init() {
        LOG_INFO("Creating window {0} with size {1}x{2}",
            data_.title, data_.width, data_.height);

        if (!isGLFWinitialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW");
                return -1;
            }
        }
            
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(),
            nullptr, nullptr);
        
        if (!window_)
        {
            LOG_CRITICAL("Can't create window {0}", data_.title);
            glfwTerminate();
            return -2;
        }
       
        isGLFWinitialized = true;

        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

        // ������������� ���������������� ������, ��� ������������� ��� window
        glfwSetWindowUserPointer(window_, &data_);

        glfwSetWindowSizeCallback(window_, 
            [](GLFWwindow* window, int width, int height) {   
                // ������ (void*) -> (WindowData*), � �� ����� ������������ 
                // ���������������� ������ ��� window
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                // ��������� ������
                data.width = width;
                data.height = height;

                // ��������� event ��� ���� ����� �������� � ����������� callback
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(window_,
            [](GLFWwindow* window, double x, double y) {
                // ������ (void*) -> (WindowData*), � �� ����� ������������ 
                // ���������������� ������ ��� window
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                // ��������� event ��� ���� ����� �������� � ����������� callback
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(window_,
            [](GLFWwindow* window) {
                // ������ (void*) -> (WindowData*), � �� ����� ������������ 
                // ���������������� ������ ��� window
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                // ��������� event ��� ���� ����� �������� � ����������� callback
                EventWindowClose event;
                data.eventCallbackFn(event);
            }        
        );


        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(window_);
        glfwTerminate();
	}


}
