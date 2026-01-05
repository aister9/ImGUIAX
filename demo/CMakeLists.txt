add_executable (ImGuiAXExample
    "ApplicationExample.cpp"
)
set_target_properties(ImGuiAXExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output
)
target_link_libraries(ImGuiAXExample PRIVATE
    visualizerLib
)
target_include_directories(ImGuiAXExample PUBLIC
     ${PROJECT_SOURCE_DIR}/libs
)
