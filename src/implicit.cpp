#include <implicit_shader/implicit.h>
#include "Shader.h"

namespace implicit_shader {

ImplicitFunction::ImplicitFunction(const std::string shader_path, size_t num_parameters)
{
    ImplicitFunction::m_shader = std::make_unique<Shader>();
    auto& shader = *m_shader;
    shader.onInit(shader_path, num_parameters, ImplicitFunction::BATCH_SIZE / 4);
}

ImplicitFunction::~ImplicitFunction()
{
    if (m_shader) {
        auto& shader = *m_shader;
        shader.onFinish();
    }
}

void ImplicitFunction::evaluate(std::span<Scalar> buffer)
{
    if (m_shader == nullptr) {
        throw std::runtime_error("Shader is not initialized");
    }
    auto& shader = *m_shader;
    const size_t num_batches =
        buffer.size() / BATCH_SIZE + (buffer.size() % BATCH_SIZE == 0 ? 0 : 1);
    for (size_t i = 0; i < num_batches; i++) {
        size_t buffer_size = std::min(BATCH_SIZE, buffer.size() - i * BATCH_SIZE);
        assert(buffer_size % 4 == 0);
        auto sub_buffer = buffer.subspan(i * BATCH_SIZE, buffer_size);
        shader.onCompute(get_parameters(), sub_buffer);
    }
}


} // namespace implicit_shader
