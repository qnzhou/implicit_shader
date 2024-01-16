#include <iostream>

#include <implicit_shader/Application.h>

int main(int, char**) {
    implicit_shader::Application app;
    app.onInit(RESOURCE_DIR "/compute-shader.wgsl");
    app.onCompute();
    app.onFinish();
    return 0;
}
