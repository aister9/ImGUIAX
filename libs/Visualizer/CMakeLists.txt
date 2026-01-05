add_library(visualizerLib
    UI/FileDialog.cpp
)

target_link_libraries(visualizerLib PUBLIC
    imgui::imgui
    glfw
    GLEW::GLEW
    OpenGL::GL
)

target_include_directories(visualizerLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
