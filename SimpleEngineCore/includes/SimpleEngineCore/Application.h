#pragma once
#include <memory>

#include "SimpleEngineCore/Event.h"
#include "SimpleEngineCore/Camera.h"


namespace SimpleEngine {

    class Application {
    private:
        std::unique_ptr<class Window> window_;
        EventDispatcher eventDispatcher_;

        bool isCloseWindow_ = false;
        
        


    private:



    public:
        Application();
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        virtual int Start(unsigned int window_width, unsigned int window_height, const char* title);

        virtual void OnUpdate() {}
        virtual void OnUIDraw() {}





        float camera_position[3] = { 0.f, 0.f, 1.f };
        float camera_rotation[3] = { 0.f, 0.f, 0.f };
        bool perspective_camera = true;
        Camera camera;



    };

}