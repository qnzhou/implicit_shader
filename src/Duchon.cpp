#include <implicit_shader/Duchon.h>

#include "Shader.h"

#include <algorithm>
#include <cassert>
#include <exception>

namespace implicit_shader {

Duchon::Duchon(
    std::span<Scalar> control_points,
    std::span<Scalar> rbf_coeffs,
    std::span<Scalar> affine_coeffs)
    : ImplicitFunction(SHADER_DIR "/duchon.wgsl", control_points.size() / 3 * 7 + 4)
{
    const size_t n = control_points.size() / 3;
    m_parameters.resize(n * 7 + 4);
    std::copy(control_points.begin(), control_points.end(), m_parameters.begin());
    std::copy(rbf_coeffs.begin(), rbf_coeffs.end(), m_parameters.begin() + n * 3);
    std::copy(affine_coeffs.begin(), affine_coeffs.end(), m_parameters.begin() + n * 7);
}

} // namespace implicit_shader
