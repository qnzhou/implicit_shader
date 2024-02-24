#pragma once

#include <implicit_shader/implicit.h>

#include <array>

namespace implicit_shader {

/**
 * Implicit sphere function.
 */
class Sphere final : public ImplicitFunction
{
public:
    /**
     * Sphere constructor.
     *
     * @param center    Sphere center.
     * @param radius    Sphere radius.
     */
    Sphere(const Point& center, Scalar radius);

protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    std::span<Scalar> get_parameters() override { return m_parameters; }

private:
    std::array<Scalar, 4> m_parameters;
};


/**
 * Implicit cylinder function.
 */
class Cylinder final : public ImplicitFunction
{
public:
    /**
     * Cylinder constructor.
     *
     * @param p1     First axis point of the cylinder.
     * @param p2     Second axis point of the cylinder.
     * @param radius Cylinder radius.
     */
    Cylinder(const Point& p1, const Point& p2, Scalar radius);

protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    std::span<Scalar> get_parameters() override { return m_parameters; }

private:
    std::array<Scalar, 7> m_parameters;
};

class Plane final: public ImplicitFunction
{
public:
    /**
     * Plane constructor.
     *
     * @param p     A point on the plane.
     * @param n     The normal of the plane.
     */
    Plane(const Point& p, const Point& n);

protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    std::span<Scalar> get_parameters() override { return m_parameters; }

private:
    std::array<Scalar, 6> m_parameters;
};


} // namespace implicit_shader
