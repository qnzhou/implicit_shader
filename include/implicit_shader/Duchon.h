#pragma once

#include <implicit_shader/implicit.h>

#include <span>
#include <vector>

namespace implicit_shader {

/**
 * This class implements the 3D Duchon's interpolant (aka Hermit RBF interpolant).
 *
 * @see Equation 3 in [1].
 *
 * [1] Huang, Zhiyang, Nathan Carr, and Tao Ju. "Variational implicit point set surfaces." ACM
 * Transactions on Graphics (TOG) 38.4 (2019): 1-13.
 */
class Duchon final : public ImplicitFunction
{
public:
    /**
     * Constructing a Duchon interpolant.
     *
     * @param control_points[in] A n*dim array representing control points stored in row major.
     * @param rbf_coeffs[in]     A n*(dim+1) array representing RBF (i.e. non-affine) coefficients
     * of the interpolant.
     * @param affine_coeffs[in]  A (dim+1) array representing affine coefficients of the
     * interpolant.
     *
     * @note `rbf_coeffs` corresponds to the `a` and `b` parameter in equation 3 of [1],
     *       where `a` is `n` vector and `b` is a (n x dim) matrix stored in column major.
     *       `affine_coeffs` corresponds to the `c` and `d` parameter in equation 3 of [1],
     *       where `c` is a vector of size dim and `d` is a scalar.
     */
    Duchon(
        std::span<Scalar> control_points,
        std::span<Scalar> rbf_coeffs,
        std::span<Scalar> affine_coeffs);

protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    std::span<Scalar> get_parameters() override { return m_parameters; }

private:
    std::vector<Scalar> m_parameters;
};

} // namespace implicit_shader
