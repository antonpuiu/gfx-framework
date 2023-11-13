#include "square.h"

#include "core/gpu/vertex_format.h"
#include "components/simple_scene.h"

using namespace gfxc;

Square::Square()
    : Square(VertexColor::WHITE, true)
{
}

Square::Square(glm::vec3 color, bool fill)
    : color(color)
    , fill(fill)
{
}

Square::~Square()
{
}

void Square::Init()
{
    std::string name = "square" + std::to_string(color.x) + std::to_string(color.y) +
                       std::to_string(color.z) + std::to_string(fill);

    if (SimpleScene::meshes.find(name) != SimpleScene::meshes.end()) {
        mesh = SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices{
        { { .5f, .5f, 0 }, color }, // 0
        { { -.5f, .5f, 0 }, color }, // 1
        { { -.5f, -.5f, 0 }, color }, // 2
        { { .5f, -.5f, 0 }, color } // 3
    };

    std::vector<unsigned int> indices;

    if (fill) {
        indices = {
            0, 1, 2, // first triangle
            2, 3, 0 // second triangle
        };

        mesh->InitFromData(vertices, indices);
    } else {
        indices = { 0, 1, 2, 3 };

        mesh->SetDrawMode(GL_LINE_LOOP);
        mesh->InitFromData(vertices, indices);
    }

    SimpleScene::meshes[name] = mesh;
}
