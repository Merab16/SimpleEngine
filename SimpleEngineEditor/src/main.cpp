#include <iostream>
#include <memory>

#include <SimpleEngineCore/Application.h>
#include <imgui/imgui.h>

#include "SimpleEngineCore/Input.h"


class SimpleEnigneEditor : public SimpleEngine::Application
{
    virtual void OnUpdate() override
    {   
        //bool mvoe_camera = false;
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };



        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_W)) {
            movement_delta.x += 0.05f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_S)) {
            movement_delta.x -= 0.05f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_A)) {
            movement_delta.y -= 0.05f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_D)) {
            movement_delta.y += 0.05f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_E)) {
            movement_delta.z += 0.05f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_Q)) {
            movement_delta.z -= 0.05f;
        }


        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_UP)) {
            rotation_delta.y -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_DOWN)) {
            rotation_delta.y += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_RIGHT)) {
            rotation_delta.z -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_LEFT)) {
            rotation_delta.z += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_P)) {
            rotation_delta.x -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_O)) {
            rotation_delta.x += 0.5f;
        }

        camera.AddMovementAndRotation(movement_delta, rotation_delta);

    }

    virtual void OnUIDraw() override {

        camera_position[0] = camera.GetCameraPosition().x;
        camera_position[1] = camera.GetCameraPosition().y;
        camera_position[2] = camera.GetCameraPosition().z;

        camera_rotation[0] = camera.GetCameraRotation().x;
        camera_rotation[1] = camera.GetCameraRotation().y;
        camera_rotation[2] = camera.GetCameraRotation().z;



        ImGui::Begin("Editor UI");
        if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f)) {
            camera.SetPosition(glm::vec3{camera_position[0], camera_position[1], camera_position[2] });
        }
        if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f)) {
            camera.SetRotation(glm::vec3{ camera_rotation[0], camera_rotation[1], camera_rotation[2] });
        }
        ImGui::Checkbox("Perspective camera", &perspective_camera);
        ImGui::End();

    }

    int frame = 0;
};


int main()
{
    auto myApp = std::make_unique<SimpleEnigneEditor>();

    int returnCode = myApp->Start(1600, 900, "SimpleEnigneEditor");

    std::cin.get();

    return returnCode;
}