#pragma once

#include <implicit_shader/implicit.h>

#include <array>

namespace implicit_shader {

using Point = std::array<Scalar, 3>;

/**
 * Implicit sphere function.
 */
class Sphere final : public ImplicitFunction
{
public:
    Sphere(const Point& center, Scalar radius);

    void evaluate(std::span<Scalar> buffer) override;

private:
    std::array<Scalar, 4> m_parameters;
};


} // namespace implicit_shader
