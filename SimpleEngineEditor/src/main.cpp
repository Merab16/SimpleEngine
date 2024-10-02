#include <iostream>
#include <memory>

#include <SimpleEngineCore/Application.hpp>

class MyApp : public SimpleEngine::Application
{
    virtual void OnUpdate() override
    {
    //    std::cout << "Update frame: " << frame++ << std::endl;
    }

    int frame = 0;
};


int main()
{
    auto myApp = std::make_unique<MyApp>();

    int returnCode = myApp->Start(800, 600, "My first App");

    std::cin.get();

    return returnCode;
}