#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.h"


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

    GLfloat positions_and_colors[] = {
         0.f, 0.5f, 0.f,    1.f, 0.f, 0.3f,
        0.5f, -0.5f, 0.f,   0.8f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f,   0.f, 0.4f, 1.f
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

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> points_vbo;
    std::unique_ptr<VertexBuffer> colors_vbo;
    std::unique_ptr<VertexArray> vao;

    std::unique_ptr<VertexBuffer> positions_and_colors_vbo;
    std::unique_ptr<VertexArray> vao_one_buffer;

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

        



        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = data_.width;
        io.DisplaySize.y = data_.height;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background color", backgroundColor_);

        static bool use2buffers = false;
        ImGui::Checkbox("2 Buffers", &use2buffers);
        if (use2buffers) {
            shaderProgram->Bind();
            vao->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        else {
            shaderProgram->Bind();
            vao_one_buffer->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }


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

        BufferLayout buffer_layout_1vec3
        {
            ShaderDataType::Float3
        };

        points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points), buffer_layout_1vec3);
        colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors), buffer_layout_1vec3);
        vao = std::make_unique<VertexArray>();

        vao->AddBuffer(*points_vbo);
        vao->AddBuffer(*colors_vbo);

        BufferLayout buffer_layout_2vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        vao_one_buffer = std::make_unique<VertexArray>();
        positions_and_colors_vbo = std::make_unique<VertexBuffer>(positions_and_colors, sizeof(positions_and_colors), buffer_layout_2vec3);

        vao_one_buffer->AddBuffer(*positions_and_colors_vbo);
       



        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(window_);
        glfwTerminate();
	}


}
