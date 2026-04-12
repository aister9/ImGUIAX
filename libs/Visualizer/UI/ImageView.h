#pragma once

#include <string>

#include "Components.h"

namespace ImGUIAX
{
    namespace Visualizer
    {
        class ImageView final : public Components
        {
        public:
            ImageView();
            explicit ImageView(const char *title);

            void setTitle(const char *title);
            const char *title() const;

            void setTexture(ImTextureID texture, const ImVec2 &size);
            void clearTexture();
            bool hasTexture() const;

            void setFitToWindow(bool enabled);
            void setFitToParent(bool enabled);
            void setKeepAspect(bool enabled);
            void setShowInfo(bool enabled);
            void setIndependent(bool enabled);

            void draw() override;

        private:
            std::string m_title = "Image View";
            ImTextureID m_texture = (ImTextureID)0;
            ImVec2 m_textureSize = ImVec2(0.0f, 0.0f);
            bool m_fitToWindow = true;
            bool m_fitToParent = true;
            bool m_keepAspect = true;
            bool m_showInfo = true;
            bool m_independence = true;
        };
    }
}
