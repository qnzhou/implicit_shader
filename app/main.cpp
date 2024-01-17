#include <implicit_shader/Application.h>

#include <iostream>
#include <vector>

int main(int, char**) {
    implicit_shader::Application app;
    constexpr size_t N = 8 * 8 * 8;
    std::vector<float> data(N * 4);
    for (size_t i = 0; i < N; i++) {
        float x = i / (8 * 8);
        float y = (i / 8) % 8;
        float z = i % 8;
        data[i * 4] = (x - 4) / 4.0f;
        data[i * 4 + 1] = (y - 4) / 4.0f;
        data[i * 4 + 2] = (z - 4) / 4.0f;
        data[i * 4 + 3] = -1;
    }
    app.onInit(RESOURCE_DIR "/cube.wgsl", N);
    app.onCompute(data);
    app.onFinish();
    return 0;
}
