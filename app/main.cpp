#include <implicit_shader/Application.h>
#include <mshio/mshio.h>

#include <iostream>
#include <vector>

struct Grid {
    std::vector<float> vertices;
    std::vector<uint32_t> hexes;

    size_t num_vertices() const { return vertices.size() / 4; }
};

Grid generate_grid(uint32_t NX, uint32_t NY, uint32_t NZ, float sx=1, float sy=1, float sz=1) {
    Grid grid;
    grid.vertices.resize((NX + 1) * (NY + 1) * (NZ + 1) * 4);
    grid.hexes.resize(NX * NY * NZ * 8);

    uint32_t N = std::min(NX, std::min(NY, NZ));

    for (uint32_t i = 0; i <= NX; i++) {
        for (uint32_t j = 0; j <= NY; j++) {
            for (uint32_t k = 0; k <= NZ; k++) {
                uint32_t idx = i * (NY + 1) * (NZ + 1) + j * (NZ + 1) + k;
                grid.vertices[idx * 4] = (i / (float)N - (float)NX / (float)N * 0.5f) * sx;
                grid.vertices[idx * 4 + 1] = (j / (float)N - (float)NY / (float)N * 0.5f) * sy;
                grid.vertices[idx * 4 + 2] = (k / (float)N - (float)NZ / (float)N * 0.5f) * sz;
                grid.vertices[idx * 4 + 3] = -1;
            }
        }
    }

    for (uint32_t i = 0; i < NX; i++) {
        for (uint32_t j = 0; j < NY; j++) {
            for (uint32_t k = 0; k < NZ; k++) {
                uint32_t idx = i * NY * NZ + j * NZ + k;
                grid.hexes[idx * 8] =
                    i * (NY + 1) * (NZ + 1) + j * (NZ + 1) + k;
                grid.hexes[idx * 8 + 1] =
                    i * (NY + 1) * (NZ + 1) + j * (NZ + 1) + k + 1;
                grid.hexes[idx * 8 + 2] =
                    i * (NY + 1) * (NZ + 1) + (j + 1) * (NZ + 1) + k + 1;
                grid.hexes[idx * 8 + 3] =
                    i * (NY + 1) * (NZ + 1) + (j + 1) * (NZ + 1) + k;
                grid.hexes[idx * 8 + 4] =
                    (i + 1) * (NY + 1) * (NZ + 1) + j * (NZ + 1) + k;
                grid.hexes[idx * 8 + 5] =
                    (i + 1) * (NY + 1) * (NZ + 1) + j * (NZ + 1) + k + 1;
                grid.hexes[idx * 8 + 6] =
                    (i + 1) * (NY + 1) * (NZ + 1) + (j + 1) * (NZ + 1) + k + 1;
                grid.hexes[idx * 8 + 7] =
                    (i + 1) * (NY + 1) * (NZ + 1) + (j + 1) * (NZ + 1) + k;
            }
        }
    }

    return grid;
}

void save_grid(const Grid& grid) {
    const auto num_vertices = grid.num_vertices();
    const auto num_hexes = grid.hexes.size() / 8;

    mshio::MshSpec spec;
    auto& format = spec.mesh_format;
    format.file_type = 1;

    // Save nodes
    {
        auto& nodes = spec.nodes;
        nodes.num_entity_blocks = 1;
        nodes.num_nodes = num_vertices;
        nodes.min_node_tag = 1;
        nodes.max_node_tag = num_vertices;
        nodes.entity_blocks.resize(1);

        auto& block = nodes.entity_blocks[0];
        block.entity_dim = 3;
        block.entity_tag = 1;
        block.num_nodes_in_block = num_vertices;
        block.tags.resize(num_vertices);
        for (size_t i = 0; i < num_vertices; i++) {
            block.tags[i] = i + 1;
        }
        block.data.resize(num_vertices * 3);
        for (size_t i = 0; i < num_vertices; i++) {
            block.data[i * 3] = grid.vertices[i * 4];
            block.data[i * 3 + 1] = grid.vertices[i * 4 + 1];
            block.data[i * 3 + 2] = grid.vertices[i * 4 + 2];
        }
    }

    // Save hexes
    {
        auto& elements = spec.elements;
        elements.num_entity_blocks = 1;
        elements.num_elements = num_hexes;
        elements.min_element_tag = 1;
        elements.max_element_tag = elements.num_elements;
        elements.entity_blocks.resize(1);

        auto& block = elements.entity_blocks[0];
        block.entity_dim = 3;
        block.entity_tag = 1;
        block.element_type = 5;
        block.num_elements_in_block = elements.num_elements;
        block.data.resize(elements.num_elements * 9);
        for (size_t i = 0; i < num_hexes; i++) {
            block.data[i * 9] = i + 1;
            for (size_t j = 0; j < 8; j++) {
                block.data[i * 9 + j + 1] = grid.hexes[i * 8 + j] + 1;
            }
        }
    }

    // Save sdf
    {
        spec.node_data.resize(1);
        auto& data = spec.node_data[0];
        auto& header = data.header;
        header.string_tags = {"sdf"};
        header.real_tags = {0};
        header.int_tags = {0, 1, static_cast<int>(num_vertices), 0};

        auto& data_entries = data.entries;
        data_entries.resize(num_vertices);
        for (size_t i = 0; i < num_vertices; i++) {
            auto& entry = data_entries[i];
            entry.tag = i + 1;
            entry.data = {grid.vertices[i * 4 + 3]};
        }
    }

    mshio::save_msh("grid.msh", spec);
}

int main(int, char**) {
    constexpr size_t NX = 64, NY = 64, NZ = 64;
    auto grid = generate_grid(NX, NY, NZ, 2, 2, 2);
    const auto num_vertices = grid.num_vertices();

    implicit_shader::Application app;
    app.onInit(SHADER_DIR "/cables/cable_0.wgsl", num_vertices);
    app.onCompute(grid.vertices);
    app.onFinish();

    save_grid(grid);
    return 0;
}
