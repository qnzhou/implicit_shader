# Implicit Shader

This library provides evaluation function for implicit surfaces defined as WGSL shaders.

Here is the code snippet for evaluating the function at an array of points.

```c++
#include <implicit_shader/Application.h>

// Input/output buffer.
// The x,y,z values are the evaluation points.
// The d values are used to store the evaluation result.
std::vector<float> buffer = {
    x0, y0, z0, d0,
    x1, y1, z1, d1,
    ...
};
size_t num_pts = buffer.size() / 4;

// Invoke the shader.
implicit_shader::Application app;
app.onInit(shader_filename, num_pts);
app.onCompute(buffer);
app.onFinish();

// The d values `buffer` has been udpated.
```
