#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"


namespace SimpleEngine {
    static bool isGLFWinitialized = false;

	// constr & destr
	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: width_(width)
		, height_(height)
		, title_(title)
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
        LOG_INFO("Creating window {0} with size {1}x{2}", title_, width_, height_);

        if (!isGLFWinitialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW");
                return -1;
            }
        }
            
        window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        
        if (!window_)
        {
            LOG_CRITICAL("Can't create window {0}", title_);
            glfwTerminate();
            return -2;
        }
       
        isGLFWinitialized = true;

        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(window_);
        glfwTerminate();
	}


}
