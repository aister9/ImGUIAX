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
#include "Visualizer/UI/ImageView.h"

#include <GL/glew.h>
#include <imgui.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using SPIN::Visualizer::Components;
using SPIN::Visualizer::Stage;

class ExampleGUIPanel final : public Components
{
public:
    ExampleGUIPanel()
    {
        imgView.setIndependent(false);
        imgView.setFitToParent(true);
        imgView.setKeepAspect(true);
        imgView.setShowInfo(true);
        loadTexture("checkboard.png");
    }

    ~ExampleGUIPanel() override
    {
        if (textureId != 0)
        {
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }
    }

    void draw() override
    {
        ImGui::SetNextWindowPos(ImVec2(32, 32), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(520, 500), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Example GUI Panel"))
        {
            imgView.draw();
        }
        ImGui::End();
    }

private:
    void loadTexture(const char *path)
    {
        stbi_uc *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
        {
            return;
        }

        if (textureId != 0)
        {
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        imgView.setTexture((ImTextureID)(intptr_t)textureId,
                           ImVec2(static_cast<float>(width), static_cast<float>(height)));
    }

    SPIN::Visualizer::ImageView imgView;
    GLuint textureId = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
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
