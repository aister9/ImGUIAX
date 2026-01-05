#include <array>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <fstream>
#include <algorithm>

#include "Visualizer/Application.h"
#include "Visualizer/UI/FileDialog.h"

using SPIN::Visualizer::Components;
using SPIN::Visualizer::Stage;

namespace
{
    std::string PathToDisplayString(const std::filesystem::path &path)
    {
#if defined(_WIN32)
        return path.u8string();
#else
        return path.string();
#endif
    }
}

class ExampleGUIPanel final : public Components
{
public:
    ExampleGUIPanel()
    {

    }

    void draw() override
    {
        ImGui::SetNextWindowPos(ImVec2(32, 32), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(520, 500), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Example GUI Panel"))
        {
            
        }
        ImGui::End();
    }

private:
    
};

class DemoApplication final : public SPIN::Visualizer::Application
{
public:
    const char *title() const override { return "ImGUIAX GUI (Stage prototype)"; }

    void start(Stage &stage) override
    {
        stage.setClearColor(initialClearColor());
        stage.emplaceDrawable<ExampleGUIPanel>();

        stage.setSceneRenderer([](Stage &, float)
                               {
                                   // Placeholder: wire the GLSL renderer here (SimpleRenderer, etc.).
                               });
    }
};

int main(int argc, char **argv)
{
    return SPIN::Visualizer::launch<DemoApplication>(argc, argv);
}
