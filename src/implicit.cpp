#include <implicit_shader/implicit.h>
#include "Shader.h"

namespace implicit_shader {

ImplicitFunction::~ImplicitFunction()
{
    if (m_shader) {
        auto& shader = *m_shader;
        shader.onFinish();
    }
}

} // namespace implicit_shader
