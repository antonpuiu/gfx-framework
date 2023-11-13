#include "hexagon.h"

#include "core/gpu/vertex_format.h"
#include "components/simple_scene.h"

using namespace gfxc;

Hexagon::Hexagon()
    : Hexagon(VertexColor::WHITE)
{
}

Hexagon::Hexagon(glm::vec3 color)
    : color(color)
{
}

Hexagon::~Hexagon()
{
}

void Hexagon::Init()
{
    std::string name =
        "hexagon" + std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

    if (SimpleScene::meshes.find(name) != SimpleScene::meshes.end()) {
        mesh = SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices;

    int sides = 6;
    float radius = .5f;

    for (int i = 0; i < sides; i++) {
        float angle = M_PI * 2.0f / sides * i - M_PI / 2;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        vertices.push_back({ { x, y, 0 }, color });
    }

    std::vector<unsigned int> indices{
        0, 1, 2, // First Triangle
        2, 3, 4, // Second Triangle
        4, 5, 0, // Third Triangle
        0, 2, 4 // Fourth Triangle
    };

    mesh->InitFromData(vertices, indices);

    SimpleScene::meshes[name] = mesh;
}
