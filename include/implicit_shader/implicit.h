#pragma once

#include <memory>
#include <span>

namespace implicit_shader {
using Scalar = float;

class Shader;

class ImplicitFunction
{
public:
    static constexpr size_t BATCH_SIZE = 1024;

public:
    virtual ~ImplicitFunction();

public:
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
    virtual void evaluate(std::span<Scalar> buffer) = 0;

protected:
    std::shared_ptr<Shader> m_shader;
};

} // namespace implicit_shader
