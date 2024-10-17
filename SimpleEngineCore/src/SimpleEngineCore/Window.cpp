#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.h"


namespace SimpleEngine {
    static bool isGLFWinitialized = false;

    GLfloat points[]{
        0.f, 0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        -0.5f, -0.5f, 0.f
    };

    GLfloat colors[]{
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };

     const char* vertex_shader = 
         "#version 450\n"
         "layout(location = 0) in vec3 vertex_position;"
         "layout(location = 1) in vec3 vertex_color;"
         "out vec3 color;"
         "void main() {"
         "  color = vertex_color;"
         "  gl_Position = vec4(vertex_position, 1.f);"
         "}";
     const char* fragment_shader =
         "#version 460\n"
         "in vec3 color;"
         "out vec4 frag_color;"
         "void main() {"
         "  frag_color = vec4(color, 1.f);"
         "}";

    GLuint vao;
    std::unique_ptr<ShaderProgram> shaderProgram;

	// constr & destr
	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: data_({width, height, title})
	{
		
		int res = Init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
	}
	Window::~Window() {
		Shutdown();
	}

	// public
    void Window::OnUpdate() {
        glClearColor(backgroundColor_[0], backgroundColor_[1],
            backgroundColor_[2], backgroundColor_[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->Bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = data_.width;
        io.DisplaySize.y = data_.height;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background color", backgroundColor_);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



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

        glfwSetFramebufferSizeCallback(window_,
            [](GLFWwindow* window, int width, int height) {
                glViewport(0, 0, width, height);
            }
        );
       

        shaderProgram = std::make_unique<ShaderProgram>(vertex_shader, 
                fragment_shader);
        if (!shaderProgram->IsCompiled()) {
            return false;
        }

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(window_);
        glfwTerminate();
	}


}
