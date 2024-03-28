# Implicit Shader

This library provides function and gradient evaluation for implicit surfaces defined as WGSL shaders.
The following types of implicit surface are supported:
* Sphere
* Cylinder
* Plane
* Cone
* Torus
* Capsule
* Duchon's interpolant (i.e. [VIPSS](https://www.cs.wustl.edu/~taoju/research/vipss.pdf))

## Creation

To create primitive implicit surfaces:

```c++
#include <implicit_shader/primitives.h>

using namespace implicit_shader;

Sphere fn({0, 0, 0}, 1); // center, radius
Cylinder fn({0, 0, 0}, {1, 0, 0}, 0.5); // end point 1, end point 2, radius
Plane fn({0, 0, 0}, {0, 0, 1}); // point, normal
Cone fn({0, 0, 0}, {0, 0, 1}, M_PI / 2); // apex, axis, angle (in radians)
Torus fn({0, 0, 0}, {0, 0, 1}, 2, 0.5); // center, axis, major radius, minor radius
Capsule fn({0, 0, 0}, {1, 0, 0}, 0.5); // end point 1, end point 2, radius
```

To create Duchon's interpolant:

```c++
#include <implicit_shader/Duchon.h>

using namespace implicit_shader;

Duchon fn(
    {...}, // A flat array of control points (row major).
    {...}, // A flat array of RBF coefficients (column major).
    {...}  // A flat array of affine coefficients (column major).
);
```

## Usage

```c++
// Input/output buffer
// As input, (xi, yi, zi) are the query points.
// As ouptut, wi is the function value, (xi, yi, zi) is the function gradient.
std::vector<float> buffer = {x0, y0, z0, w0, x1, y1, z1, w1, ...};

fn.evaluate(buffer);
```
