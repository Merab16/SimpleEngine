#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"
#include "Camera.h"

#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.h"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.h"
#include "SimpleEngineCore/Rendering/OpenGL/Renderer_OpenGL.h"
#include "SimpleEngineCore/Modules/UIModule.h"

namespace SimpleEngine {
    GLfloat positions_and_colors[] = {
       0.f, 0.5f, 0.f,     1.f, 0.f, 0.3f,
       0.5f, -0.5f, 0.f,   0.8f, 1.f, 0.f,
       -0.5f, -0.5f, 0.f,  0.f, 0.4f, 1.f
    };

    GLfloat positions_and_colors_rectangle[] = {
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 1.0f,

        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        -0.5, 0.5f, 0.0f,       1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 1.0f
    };

    GLfloat positions_and_colors_rectangle1[] = {
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.1f, 0.4f, 1.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.4f,
        0.5f, 0.5f, 0.0f,       0.3f, 0.0f, 0.7f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    

     const char* vertex_shader = 
         "#version 460\n"
         "layout(location = 0) in vec3 vertex_position;"
         "layout(location = 1) in vec3 vertex_color;"
         "uniform mat4 model_matrix;"
         "uniform mat4 view_projection_matrix;"
         "out vec3 color;"
         "void main() {"
         "  color = vertex_color;"
         "  gl_Position = view_projection_matrix *  model_matrix * vec4(vertex_position, 1.f);"
         "}";

     const char* fragment_shader =
         "#version 460\n"
         "in vec3 color;"
         "out vec4 frag_color;"
         "void main() {"
         "  frag_color = vec4(color, 1.f);"
         "}";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> positions_and_colors_vbo;
    std::unique_ptr<VertexArray> vao_one_buffer;
    float scale[] = { 1, 1, 1 };
    float rotate = 0.f;
    float translate[] = { 0.f, 0.f, 0.f };

    float camera_position[3] = { 0.f, 0.f, 1.f };
    float camera_rotation[3] = { 0.f, 0.f, 0.f };
    bool perspective_camera = false;
    Camera camera;

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
        Renderer_OpenGL::SetClearColor(backgroundColor_[0], backgroundColor_[1],
            backgroundColor_[2], backgroundColor_[3]);
        Renderer_OpenGL::Clear();

        
        UIModule::OnWindowUpdateBegin();

        bool show = true;
        UIModule::ShowExampleAppDockSpace(&show);

        

        

        //ImGui::ShowDemoWindow();

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background color", backgroundColor_);
        ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("translate", translate, -1.f, 1.f);

        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);
        ImGui::End();

        shaderProgram->Bind();

        glm::mat4 scale_matrix(scale[0],    0,          0,          0,
                               0,           scale[1],   0,          0,
                               0,           0,          scale[2],   0,
                               0,           0,          0,          1
            );

        float rotate_in_radian = glm::radians(rotate);
        glm::mat4 rotate_matrix(cos(rotate_in_radian),  sin(rotate_in_radian),  0, 0,
                                -sin(rotate_in_radian), cos(rotate_in_radian),  0, 0,
                                0,                      0,                      1, 0,
                                0,                      0,                      0, 1
        );

        glm::mat4 translate_matrix(
            1,              0,              0,              0,
            0,              1,              0,              0,
            0,              0,              1,              0,
            translate[0],   translate[1],   translate[2],   1
        );

        glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
        shaderProgram->SetMatrix4("model_matrix", model_matrix);

        camera.SetPositionRotation(glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
                                    glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
        camera.SetProjectionMode(
            perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic
        );

        shaderProgram->SetMatrix4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

        Renderer_OpenGL::Draw(*vao_one_buffer);

        


        UIModule::OnWindowUpdateDraw();



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
                Renderer_OpenGL::SetViewport(width, height);
            }
        );
       
        UIModule::OnWindowCreate(window_);



        shaderProgram = std::make_unique<ShaderProgram>(vertex_shader, 
                fragment_shader);
        if (!shaderProgram->IsCompiled()) {
            return false;
        }

        BufferLayout buffer_layout_2vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        vao_one_buffer = std::make_unique<VertexArray>();
        positions_and_colors_vbo = std::make_unique<VertexBuffer>(positions_and_colors_rectangle1, sizeof(positions_and_colors_rectangle1), buffer_layout_2vec3);
        indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        vao_one_buffer->AddVertexBuffer(*positions_and_colors_vbo);
        vao_one_buffer->SetIndexBuffer(*indexBuffer);
       

        return 0;
	}

	void Window::Shutdown() {
        UIModule::OnWindowClose();
        glfwDestroyWindow(window_);
        glfwTerminate();

        
	}


}
