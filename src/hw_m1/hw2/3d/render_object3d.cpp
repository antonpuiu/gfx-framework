#include "render_object3d.h"
#include "components/simple_scene.h"
#include "core/gpu/vertex_format.h"

using namespace m1;

RenderObject3D::RenderObject3D()
    : RenderObject3D(VertexColor::WHITE)
{
}

Cube::Cube()
    : Cube(VertexColor::WHITE)
{
}

Cylinder::Cylinder()
    : Cylinder(VertexColor::WHITE)
{
}

Sphere::Sphere()
    : Sphere(VertexColor::WHITE)
{
}

RenderObject3D::RenderObject3D(glm::vec3 color)
    : mesh(nullptr)
    , color(color)
{
}

Cube::Cube(glm::vec3 color)
    : RenderObject3D(color)
{
}

Cylinder::Cylinder(glm::vec3 color)
    : RenderObject3D(color)
{
}

Sphere::Sphere(glm::vec3 color)
    : RenderObject3D(color)
{
}

RenderObject3D::RenderObject3D(RenderObject3D& renderObject3D)
    : Transform3D(renderObject3D)
    , mesh(renderObject3D.mesh)
    , color(renderObject3D.color)
{
}

Cube::Cube(Cube& cube)
    : RenderObject3D(cube)
{
}

Cylinder::Cylinder(Cylinder& cube)
    : RenderObject3D(cube)
{
}

Sphere::Sphere(Sphere& sphere)
    : RenderObject3D(sphere)
{
}

RenderObject3D::~RenderObject3D()
{
}

glm::vec3 RenderObject3D::GetColor()
{
    return color;
}

void RenderObject3D::SetColor(glm::vec3 color)
{
    this->color = color;

    if (mesh != nullptr)
        Init();
}

Mesh* RenderObject3D::GetMesh()
{
    return mesh;
}

RenderObject3D* Cube::Create()
{
    return new Cube();
}

RenderObject3D* Cylinder::Create()
{
    return new Cylinder();
}

RenderObject3D* Sphere::Create()
{
    return new Sphere();
}

RenderObject3D* Cube::Clone()
{
    return new Cube(*this);
}

RenderObject3D* Cylinder::Clone()
{
    return new Cylinder(*this);
}

RenderObject3D* Sphere::Clone()
{
    return new Sphere(*this);
}

void Cube::Init()
{
    std::string name =
        "cube" + std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

    if (gfxc::SimpleScene::meshes.find(name) != gfxc::SimpleScene::meshes.end()) {
        mesh = gfxc::SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices{
        VertexFormat(glm::vec3(-0.5, -0.5, 0.5), VertexColor::RED), // 0
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5), VertexColor::RED), // 1
        VertexFormat(glm::vec3(0.5, -0.5, -0.5), VertexColor::RED), // 2
        VertexFormat(glm::vec3(0.5, -0.5, 0.5), VertexColor::RED), // 3

        VertexFormat(glm::vec3(-0.5, 0.5, 0.5), VertexColor::BLUE), // 4
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5), VertexColor::BLUE), // 5
        VertexFormat(glm::vec3(0.5, 0.5, -0.5), VertexColor::BLUE), // 6
        VertexFormat(glm::vec3(0.5, 0.5, 0.5), VertexColor::BLUE) // 7
    };

    std::vector<unsigned int> indices = {
        1, 2, 0, // indices for first base triangle
        0, 2, 3, // indices for second base triangle
        5, 4, 6, // indices for first top triangle
        4, 7, 6, // indices for second top triangle
        2, 6, 3, // indices for first north triangle
        6, 7, 3, // indices for second north triangle
        1, 0, 5, // indices for first south triangle
        5, 0, 4, // indices for second south triangle
        0, 3, 7, // indices for first east triangle
        0, 7, 4, // indices for second east triangle
        1, 6, 2, // indices for first west triangle
        1, 5, 6, // indices for second west triangle
    };

    mesh->InitFromData(vertices, indices);
    gfxc::SimpleScene::meshes[name] = mesh;
}

void Cylinder::Init()
{
}

void Sphere::Init()
{
}
