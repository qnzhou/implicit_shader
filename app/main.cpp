#include <implicit_shader/Application.h>
#include <mshio/mshio.h>

#include <iostream>
#include <vector>

struct Grid {
    std::vector<float> vertices;
    std::vector<uint32_t> hexes;

    size_t num_vertices() const { return vertices.size() / 4; }
};

Grid generate_grid(uint32_t N) {
    Grid grid;
    grid.vertices.resize((N + 1) * (N + 1) * (N + 1) * 4);
    grid.hexes.resize(N * N * N * 8);

    for (uint32_t i = 0; i <= N; i++) {
        for (uint32_t j = 0; j <= N; j++) {
            for (uint32_t k = 0; k <= N; k++) {
                uint32_t idx = i * (N + 1) * (N + 1) + j * (N + 1) + k;
                grid.vertices[idx * 4] = i / (float)N - 0.5f;
                grid.vertices[idx * 4 + 1] = j / (float)N - 0.5f;
                grid.vertices[idx * 4 + 2] = k / (float)N - 0.5f;
                grid.vertices[idx * 4 + 3] = -1;
            }
        }
    }

    for (uint32_t i = 0; i < N; i++) {
        for (uint32_t j = 0; j < N; j++) {
            for (uint32_t k = 0; k < N; k++) {
                uint32_t idx = i * N * N + j * N + k;
                grid.hexes[idx * 8] = i * (N + 1) * (N + 1) + j * (N + 1) + k;
                grid.hexes[idx * 8 + 1] =
                    i * (N + 1) * (N + 1) + j * (N + 1) + k + 1;
                grid.hexes[idx * 8 + 2] =
                    i * (N + 1) * (N + 1) + (j + 1) * (N + 1) + k + 1;
                grid.hexes[idx * 8 + 3] =
                    i * (N + 1) * (N + 1) + (j + 1) * (N + 1) + k;
                grid.hexes[idx * 8 + 4] =
                    (i + 1) * (N + 1) * (N + 1) + j * (N + 1) + k;
                grid.hexes[idx * 8 + 5] =
                    (i + 1) * (N + 1) * (N + 1) + j * (N + 1) + k + 1;
                grid.hexes[idx * 8 + 6] =
                    (i + 1) * (N + 1) * (N + 1) + (j + 1) * (N + 1) + k + 1;
                grid.hexes[idx * 8 + 7] =
                    (i + 1) * (N + 1) * (N + 1) + (j + 1) * (N + 1) + k;
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
    implicit_shader::Application app;
    constexpr size_t N = 64;
    auto grid = generate_grid(N);
    const auto num_vertices = grid.num_vertices();

    app.onInit(RESOURCE_DIR "/key.wgsl", num_vertices);
    app.onCompute(grid.vertices);
    app.onFinish();

    save_grid(grid);
    return 0;
}
