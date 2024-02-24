#pragma once

#include <array>
#include <memory>
#include <span>
#include <string>

namespace implicit_shader {
using Scalar = float;
using Point = std::array<Scalar, 3>;


class Shader;

class ImplicitFunction
{
public:
    static constexpr size_t BATCH_SIZE = 1024;

    ImplicitFunction(const std::string shader_path, size_t num_parameters);
    virtual ~ImplicitFunction();

    /**
     * Evaluating an implicit function
     *
     * @param buffer[in,out] A buffer containing input and output data of the evaluation.
     *                       As input, buffer contains 4*N floats, the first 3 floats of each group
     *                       contains the xyz coordinates of the query point. So a total of N
     *                       evaluation will be done in parallel.
     *                       As output, buffer constains 4*N floats, the first 3 floats of each
     *                       group contains the gradient at the query point, and the last float
     *                       is the function value at the query point.
     */
    void evaluate(std::span<Scalar> buffer);


protected:
    /**
     * Get the parameters of the implicit function.
     *
     * @return A span of the parameters.
     */
    virtual std::span<Scalar> get_parameters() = 0;

protected:
    std::unique_ptr<Shader> m_shader;
};

} // namespace implicit_shader
