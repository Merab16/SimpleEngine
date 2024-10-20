#include <iostream>
#include <memory>

#include <SimpleEngineCore/Application.h>
#include <imgui/imgui.h>


class SimpleEnigneEditor : public SimpleEngine::Application
{
    virtual void OnUpdate() override
    {
    //    std::cout << "Update frame: " << frame++ << std::endl;
    }

    virtual void OnUIDraw() override {
        ImGui::Begin("Camera UI");
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f);
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