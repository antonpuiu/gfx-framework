#include "diamond.h"

#include "core/gpu/vertex_format.h"
#include "components/simple_scene.h"
#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"

using namespace m1;

Diamond::Diamond()
    : RenderObject2D()
{
}

Diamond::Diamond(glm::vec3 color)
    : RenderObject2D(color)
{
}

Diamond::~Diamond()
{
}

Diamond* Diamond::Create()
{
    return new Diamond();
}

Diamond* Diamond::Clone()
{
    return new Diamond(*this);
}

void Diamond::Init()
{
    std::string name =
        "diamond" + std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

    if (gfxc::SimpleScene::meshes.find(name) != gfxc::SimpleScene::meshes.end()) {
        mesh = gfxc::SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices{
        { { 0.f, 1.f, 0 }, color }, // 0
        { { -.5f, 0.f, 0 }, color }, // 1
        { { 0.f, -1.f, 0 }, color }, // 2
        { { .5f, 0.f, 0 }, color } // 3
    };

    std::vector<unsigned int> indices{
        0, 1, 2, // first triangle
        2, 3, 0 // second triangle
    };

    mesh->InitFromData(vertices, indices);

    gfxc::SimpleScene::meshes[name] = mesh;
}
