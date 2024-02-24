#include <implicit_shader/primitives.h>

#include "Shader.h"

#include <cassert>
#include <exception>

namespace implicit_shader {

Sphere::Sphere(const Point& center, Scalar radius)
{
    m_parameters = {center[0], center[1], center[2], radius};
    ImplicitFunction::m_shader = std::make_shared<Shader>();
    auto& shader = *ImplicitFunction::m_shader;
    shader.onInit(SHADER_DIR "/primitives/sphere.wgsl", 4, ImplicitFunction::BATCH_SIZE / 4);
}

void Sphere::evaluate(std::span<Scalar> buffer)
{
    if (ImplicitFunction::m_shader == nullptr) {
        throw std::runtime_error("Shader is not initialized");
    }
    auto& shader = *ImplicitFunction::m_shader;
    constexpr size_t batch_size = ImplicitFunction::BATCH_SIZE;
    const size_t num_batches =
        buffer.size() / batch_size + (buffer.size() % batch_size == 0 ? 0 : 1);
    for (size_t i = 0; i < num_batches; i++) {
        size_t buffer_size = std::min(
            ImplicitFunction::BATCH_SIZE,
            buffer.size() - i * ImplicitFunction::BATCH_SIZE);
        assert(buffer_size % 4 == 0);
        auto sub_buffer = buffer.subspan(i * ImplicitFunction::BATCH_SIZE, buffer_size);
        shader.onCompute(m_parameters, sub_buffer);
    }
}

} // namespace implicit_shader
