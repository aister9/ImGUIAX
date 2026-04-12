#pragma once

#include <imgui.h>

namespace ImGUIAX
{
    namespace Visualizer
    {
        class Components
        {
        public:
            virtual ~Components() = default;
            virtual void draw() = 0;
        };

        class DemoComponent : public Components
        {
        public:
            void draw() override
            {
                ImGui::Begin("Demo Component");
                ImGui::Text("Hello from Demo Component!");
                ImGui::End();
            }
        };
    }
}
