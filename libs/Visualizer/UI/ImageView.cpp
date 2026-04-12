#include "ImageView.h"

#include <algorithm>

namespace ImGUIAX
{
    namespace Visualizer
    {
        ImageView::ImageView() = default;

        ImageView::ImageView(const char *title)
        {
            setTitle(title);
        }

        void ImageView::setTitle(const char *title)
        {
            if (title && *title)
            {
                m_title = title;
            }
        }

        const char *ImageView::title() const
        {
            return m_title.c_str();
        }

        void ImageView::setTexture(ImTextureID texture, const ImVec2 &size)
        {
            m_texture = texture;
            m_textureSize = size;
        }

        void ImageView::clearTexture()
        {
            m_texture = (ImTextureID)0;
            m_textureSize = ImVec2(0.0f, 0.0f);
        }

        bool ImageView::hasTexture() const
        {
            return m_texture != (ImTextureID)0 && m_textureSize.x > 0.0f && m_textureSize.y > 0.0f;
        }

        void ImageView::setFitToWindow(bool enabled)
        {
            m_fitToWindow = enabled;
        }

        void ImageView::setFitToParent(bool enabled)
        {
            m_fitToParent = enabled;
        }

        void ImageView::setKeepAspect(bool enabled)
        {
            m_keepAspect = enabled;
        }

        void ImageView::setShowInfo(bool enabled)
        {
            m_showInfo = enabled;
        }

        void ImageView::setIndependent(bool enabled)
        {
            m_independence = enabled;
        }

        void ImageView::draw()
        {
            const bool useWindow = m_independence;
            if (useWindow && !ImGui::Begin(m_title.c_str()))
            {
                ImGui::End();
                return;
            }

            if (!hasTexture())
            {
                ImGui::TextDisabled("No image loaded.");
                if (useWindow)
                {
                    ImGui::End();
                }
                return;
            }

            ImVec2 avail = ImGui::GetContentRegionAvail();
            ImVec2 target = m_textureSize;

            const bool shouldFit = (useWindow && m_fitToWindow) || (!useWindow && m_fitToParent);
            if (shouldFit)
            {
                const float maxW = std::max(1.0f, avail.x);
                const float maxH = std::max(1.0f, avail.y - (m_showInfo ? ImGui::GetFrameHeightWithSpacing() : 0.0f));
                float scaleX = maxW / m_textureSize.x;
                float scaleY = maxH / m_textureSize.y;
                float scale = m_keepAspect ? std::min(scaleX, scaleY) : 1.0f;
                if (m_keepAspect)
                {
                    target = ImVec2(m_textureSize.x * scale, m_textureSize.y * scale);
                }
                else
                {
                    target = ImVec2(maxW, maxH);
                }
            }

            ImGui::Image(m_texture, target);

            if (m_showInfo)
            {
                ImGui::Separator();
                ImGui::Text("Size: %.0f x %.0f", m_textureSize.x, m_textureSize.y);
            }

            if (useWindow)
            {
                ImGui::End();
            }
        }
    }
}
