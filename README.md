# ImGUIAX

ImGUI is a powerful GUI API on its own, but I often felt the need for an OOP-style application structure. This project aims to turn ImGUI into a library that lets you organize applications in a way similar to JavaFX. The project name is ImGUI Application eXtended.

## Usage
Prerequisites:
- CMake 3.14+
- C++17 toolchain (MSVC/Clang/GCC)
- Internet access on first configure (FetchContent downloads imgui/glfw/glew)

Configure/build example:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run:
- Windows: `output/ImGuiAXExample.exe`
- Others: `./output/ImGuiAXExample`



## Library Integration (show a window like ImGuiAXExample)
Add ImGuiAX as a subdirectory and link `visualizerLib`, then create an `Application` subclass.

CMakeLists.txt:
```cmake
add_subdirectory(path/to/ImGuiAX)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE visualizerLib)
target_include_directories(MyApp PRIVATE
    ${ImGuiAX_SOURCE_DIR}/libs
)
```

main.cpp:
```cpp
#include "Visualizer/Application.h"

using SPIN::Visualizer::Application;
using SPIN::Visualizer::Stage;

class MyApp final : public Application
{
public:
    const char *title() const override { return "My ImGuiAX App"; }

    void start(Stage &stage) override
    {
        stage.setClearColor(initialClearColor());
        // stage.emplaceDrawable<YourComponent>();
    }
};

int main(int argc, char **argv)
{
    return SPIN::Visualizer::launch<MyApp>(argc, argv);
}
```

Notes:
- If you do not use CMake presets, make sure FetchContent can download dependencies on first configure.
- You can copy `demo/ApplicationExample.cpp` as a starting point.

## License
This project is licensed under the Apache License 2.0 (see `LICENSE`).

Third-party:
- Dear ImGui + backends (MIT License): see `LICENSE-3rdparty/IMGUI_LICENSE.txt`.

If you distribute binaries that bundle cuBQL, include its license notice consistent with Apache 2.0 requirements.
