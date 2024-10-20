#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <glm/trigonometric.hpp>


#include "SimpleEngineCore/Application.h"
#include "SimpleEngineCore/Window.h"
#include "SimpleEngineCore/Log.h"
#include "SimpleEngineCore/Input.h"

#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.h"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.h"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.h"
#include "SimpleEngineCore/Rendering/OpenGL/Renderer_OpenGL.h"

#include "SimpleEngineCore/Modules/UIModule.h"









#include <iostream>

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
        0.0f, -0.5f, -0.5f,      1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, -0.5f,       0.1f, 0.4f, 1.0f,
        0.0f, -0.5f, 0.5f,       1.0f, 0.0f, 0.4f,
        0.0f, 0.5f, 0.5f,        0.3f, 0.0f, 0.7f
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
    float backgroundColor[4]{ 0, 0.5, 0.5, 0 };
    



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

        eventDispatcher_.AddEventListener<EventKeyPressed>(
            [&](EventKeyPressed& event) {
                if (event.keyCode < KeyCode::KEY_Z) {
                    if (event.repeated) {
                        LOG_INFO("[Key pressed] {0}, repeated", 
                            static_cast<char>(event.keyCode));
                    }
                    else {
                        LOG_INFO("[Key pressed] {0}", 
                            static_cast<char>(event.keyCode));
                    }
                }
                Input::PressKey(event.keyCode);
            }
        );

        eventDispatcher_.AddEventListener<EventKeyReleased>(
            [&](EventKeyReleased& event) {
                if (event.keyCode < KeyCode::KEY_Z) {
                    LOG_INFO("[Key released] {0}", 
                        static_cast<char>(event.keyCode));
                }
                Input::ReleaseKey(event.keyCode);
            }
        );

        window_->SetEventCallback(
            [&](BaseEvent& event) {
                eventDispatcher_.Dispatch(event);
            }
        );

        //---------//
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




        while (!isCloseWindow_) {

            //========Figures========//
            Renderer_OpenGL::SetClearColor(backgroundColor[0], backgroundColor[1],
                backgroundColor[2], backgroundColor[3]);
            Renderer_OpenGL::Clear();

            shaderProgram->Bind();

            glm::mat4 scale_matrix(scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0, 0, 0, 1
            );

            float rotate_in_radian = glm::radians(rotate);
            glm::mat4 rotate_matrix(cos(rotate_in_radian), sin(rotate_in_radian), 0, 0,
                -sin(rotate_in_radian), cos(rotate_in_radian), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );

            glm::mat4 translate_matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                translate[0], translate[1], translate[2], 1
            );

            glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
            shaderProgram->SetMatrix4("model_matrix", model_matrix);

            camera.SetProjectionMode(
                perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic
            );

            shaderProgram->SetMatrix4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

            Renderer_OpenGL::Draw(*vao_one_buffer);



            //========Interface========//

            UIModule::OnUIDrawBegin();

            bool show = true;
            UIModule::ShowExampleAppDockSpace(&show);

            //ImGui::ShowDemoWindow();
            ImGui::Begin("Background Color Window");
            ImGui::ColorEdit4("Background color", backgroundColor);
            ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
            ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
            ImGui::SliderFloat3("translate", translate, -1.f, 1.f);

            ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
            ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f);
            ImGui::Checkbox("Perspective camera", &perspective_camera);
            ImGui::End();

            OnUIDraw();

            UIModule::OnUIDrawEnd();


            window_->OnUpdate();
            OnUpdate();
        }

        window_ = nullptr;      // � ��������� ���������, ��� ����� �������� *GLFWwindow
                                // � ����� ���������� ����������. ����� �������???????


        return 0;
    }
}