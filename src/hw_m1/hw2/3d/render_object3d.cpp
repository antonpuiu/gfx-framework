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

CylinderBase::CylinderBase()
    : CylinderBase(VertexColor::WHITE)
{
}

CylinderCover::CylinderCover()
    : CylinderBase(VertexColor::WHITE)
{
}

CylinderBody::CylinderBody()
    : CylinderBase(VertexColor::WHITE)
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

CylinderBase::CylinderBase(glm::vec3 color)
    : RenderObject3D(color)
{
}

CylinderCover::CylinderCover(glm::vec3 color)
    : CylinderBase(color)
{
}

CylinderBody::CylinderBody(glm::vec3 color)
    : CylinderBase(color)
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

void CylinderCover::Init()
{
    std::string name = "cylinder_cover" + std::to_string(color.x) + std::to_string(color.y) +
                       std::to_string(color.z);

    if (gfxc::SimpleScene::meshes.find(name) != gfxc::SimpleScene::meshes.end()) {
        mesh = gfxc::SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back({ VertexFormat(glm::vec3(0, -OFFSET, 0)) });

    for (int i = 0; i <= RESOLUTION; i++) {
        float angle = 2.f * M_PI * static_cast<float>(i) / static_cast<float>(RESOLUTION);
        vertices.push_back(VertexFormat(glm::vec3(cos(angle), -OFFSET, sin(angle)), color));
        indices.push_back(i);
    }

    indices.push_back(1);

    mesh->SetDrawMode(GL_TRIANGLE_FAN);
    mesh->InitFromData(vertices, indices);
    gfxc::SimpleScene::meshes[name] = mesh;
}

void CylinderBody::Init()
{
    std::string name = "cylinder_body" + std::to_string(color.x) + std::to_string(color.y) +
                       std::to_string(color.z);

    if (gfxc::SimpleScene::meshes.find(name) != gfxc::SimpleScene::meshes.end()) {
        mesh = gfxc::SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int idx = 0;

    for (int i = 0; i <= RESOLUTION; i++) {
        float angle = 2.f * M_PI * static_cast<float>(i) / static_cast<float>(RESOLUTION);
        vertices.push_back(VertexFormat(glm::vec3(cos(angle), 1 - OFFSET, sin(angle)), color));
        vertices.push_back(VertexFormat(glm::vec3(cos(angle), 0 - OFFSET, sin(angle)), color));

        indices.push_back(idx++);
        indices.push_back(idx++);
    }

    mesh->SetDrawMode(GL_TRIANGLE_STRIP);
    mesh->InitFromData(vertices, indices);
    gfxc::SimpleScene::meshes[name] = mesh;
}

void Sphere::Init()
{
    std::string name =
        "sphere" + std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

    if (gfxc::SimpleScene::meshes.find(name) != gfxc::SimpleScene::meshes.end()) {
        mesh = gfxc::SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int idx = 0;

    for (int i = 0; i < RESOLUTION; i++) {
        float phi1 = M_PI * static_cast<float>(i) / static_cast<float>(RESOLUTION);
        float phi2 = M_PI * static_cast<float>(i + 1) / static_cast<float>(RESOLUTION);

        for (int j = 0; j < RESOLUTION; j++) {
            float theta = 2.0f * M_PI * static_cast<float>(j) / static_cast<float>(RESOLUTION);

            float x1 = sin(phi1) * cos(theta);
            float y1 = cos(phi1);
            float z1 = sin(phi1) * sin(theta);

            float x2 = sin(phi2) * cos(theta);
            float y2 = cos(phi2);
            float z2 = sin(phi2) * sin(theta);

            vertices.push_back(VertexFormat(glm::vec3(x1, y1, z1), color));
            vertices.push_back(VertexFormat(glm::vec3(x2, y2, z2), color));

            indices.push_back(idx++);
            indices.push_back(idx++);
        }
    }

    mesh->SetDrawMode(GL_TRIANGLE_STRIP);
    mesh->InitFromData(vertices, indices);
    gfxc::SimpleScene::meshes[name] = mesh;
}
