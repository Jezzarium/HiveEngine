Hive Engine supports Windows, macOS and linux.

# Build steps

1. Clone the repository
2. Install dependencies (git submodule update --init --recursive)
3. Build the project (mkdir build && cd build && cmake -DHIVE_TRACK_PROFILE=FULL -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. && cmake --build .)
4. Run the project (cd bind && ./Testbed)


# TODO
- Graphic commands system (for rendering instead of tmp_draw)
- Mesh
- Texture
- Renderer
- Camera
- ECS
- Resource Management
- Timing (frame time)
- Job system (multi-threaded)
- UI (maybe ImGUI or something else)
- Config parser
