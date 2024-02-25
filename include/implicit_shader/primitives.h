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


/**
 * Implicit plane function.
 */
class Plane final : public ImplicitFunction
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


/**
 * Implicit cone function.
 */
class Cone final : public ImplicitFunction
{
public:
    /**
     * Cone constructor.
     *
     * @param apex  Cone apex.
     * @param axis  Cone axis.
     * @param angle Cone angle in radian.
     */
    Cone(const Point& apex, const Point& axis, Scalar angle);

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


/**
 * Implicit torus function.
 */
class Torus final : public ImplicitFunction
{
public:
    /**
     * Torus constructor.
     *
     * @param center        Torus center.
     * @param axis          Torus axis.
     * @param major_radius  Torus major radius.
     * @param minor_radius  Torus minor radius.
     */
    Torus(const Point& center, const Point& axis, Scalar major_radius, Scalar minor_radius);

protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    std::span<Scalar> get_parameters() override { return m_parameters; }

private:
    std::array<Scalar, 8> m_parameters;
};

} // namespace implicit_shader
