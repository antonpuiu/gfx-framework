#include "render_object2d.h"
#include "core/gpu/vertex_format.h"

using namespace m1;

RenderObject2D::RenderObject2D()
    : RenderObject2D(VertexColor::WHITE)
{
}

RenderObject2D::RenderObject2D(glm::vec3 color)
    : mesh(nullptr)
    , color(color)
{
}

RenderObject2D::RenderObject2D(RenderObject2D& renderObject2D)
    : Transform2D(renderObject2D)
    , mesh(renderObject2D.mesh)
    , color(renderObject2D.color)
{
}

RenderObject2D::~RenderObject2D()
{
}

void RenderObject2D::SetColor(glm::vec3 color)
{
    this->color = color;

    if (mesh != nullptr)
        Init();
}

Mesh* RenderObject2D::GetMesh()
{
    return mesh;
}
