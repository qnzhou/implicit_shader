#include <implicit_shader/primitives.h>

#include "Shader.h"

#include <cassert>
#include <exception>

namespace implicit_shader {

Sphere::Sphere(const Point& center, Scalar radius)
    : ImplicitFunction(SHADER_DIR "/primitives/sphere.wgsl", 4)
{
    m_parameters = {center[0], center[1], center[2], radius};
}

Cylinder::Cylinder(const Point& p1, const Point& p2, Scalar radius)
    : ImplicitFunction(SHADER_DIR "/primitives/cylinder.wgsl", 7)
{
    m_parameters = {p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], radius};
}

Plane::Plane(const Point& p, const Point& n)
    : ImplicitFunction(SHADER_DIR "/primitives/plane.wgsl", 6)
{
    m_parameters = {p[0], p[1], p[2], n[0], n[1], n[2]};
}

} // namespace implicit_shader
