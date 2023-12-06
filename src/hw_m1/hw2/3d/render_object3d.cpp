#include "render_object3d.h"
#include "object3d.h"
#include "components/simple_scene.h"
#include "core/gpu/vertex_format.h"
#include <algorithm>
#include <iostream>

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

bool RenderObject3D::CollisionTest(RenderObject3D* o1, RenderObject3D* o2)
{
    return false;
}

bool RenderObject3D::CollisionTest(Tank* tank, Walls* walls)
{
    glm::vec3 tankPos = tank->GetPosition();
    glm::vec3 wallsScl = walls->GetGlobalScale();

    float tankRadius = tank->GetTankRadius();

    if (tankPos.x - tankRadius >= -wallsScl.x / 2.0 && tankPos.x + tankRadius <= wallsScl.x / 2.0 &&
        tankPos.z - tankRadius >= -wallsScl.z / 2.0 && tankPos.z + tankRadius <= wallsScl.z / 2.0)
        return false;

    return true;
}

bool RenderObject3D::CollisionTest(Tank* tank, Building* building)
{
    glm::vec3 tankPos = tank->GetPosition();
    float tankRadius = tank->GetTankRadius();

    glm::vec3 buildingPos = building->GetPrimitives()[0]->GetGlobalPosition();
    glm::vec3 buildingScl = building->GetPrimitives()[0]->GetGlobalScale();

    auto buildingMin = buildingPos - (buildingScl / glm::vec3(2.0, 2.0, 2.0));
    auto buildingMax = buildingPos + (buildingScl / glm::vec3(2.0, 2.0, 2.0));

    if ((tankPos.x + tankRadius >= buildingMin.x && tankPos.x + tankRadius <= buildingMax.x &&
         tankPos.z >= buildingMin.z && tankPos.z <= buildingMax.z))
        return true;

    if ((tankPos.x - tankRadius >= buildingMin.x && tankPos.x - tankRadius <= buildingMax.x &&
         tankPos.z >= buildingMin.z && tankPos.z <= buildingMax.z))
        return true;

    if ((tankPos.z + tankRadius >= buildingMin.z && tankPos.z + tankRadius <= buildingMax.z &&
         tankPos.x >= buildingMin.x && tankPos.x <= buildingMax.x))
        return true;

    if ((tankPos.z - tankRadius >= buildingMin.z && tankPos.z - tankRadius <= buildingMax.z &&
         tankPos.x >= buildingMin.x && tankPos.x <= buildingMax.x))
        return true;

    return false;
}

bool RenderObject3D::CollisionTest(Tank* tank1, Tank* tank2)
{
    glm::vec3 tank1Pos = tank1->GetPosition();
    float tank1Radius = tank1->GetTankRadius();

    glm::vec3 tank2Pos = tank2->GetPosition();
    float tank2Radius = tank2->GetTankRadius();

    auto tank2Min = tank2Pos - glm::vec3(tank2Radius / 2, 0, tank2Radius / 2);
    auto tank2Max = tank2Pos + glm::vec3(tank2Radius / 2, 0, tank2Radius / 2);

    if ((tank1Pos.x + tank1Radius >= tank2Min.x && tank1Pos.x + tank1Radius <= tank2Max.x &&
         tank1Pos.z >= tank2Min.z && tank1Pos.z <= tank2Max.z))
        return true;

    if ((tank1Pos.x - tank1Radius >= tank2Min.x && tank1Pos.x - tank1Radius <= tank2Max.x &&
         tank1Pos.z >= tank2Min.z && tank1Pos.z <= tank2Max.z))
        return true;

    if ((tank1Pos.z + tank1Radius >= tank2Min.z && tank1Pos.z + tank1Radius <= tank2Max.z &&
         tank1Pos.x >= tank2Min.x && tank1Pos.x <= tank2Max.x))
        return true;

    if ((tank1Pos.z - tank1Radius >= tank2Min.z && tank1Pos.z - tank1Radius <= tank2Max.z &&
         tank1Pos.x >= tank2Min.x && tank1Pos.x <= tank2Max.x))
        return true;

    return false;
}

bool RenderObject3D::CollisionTest(TankProjectile* projectile, Walls* walls)
{
    glm::vec3 projectilePos = projectile->GetPosition();
    glm::vec3 projectileScl = projectile->GetPrimitives()[0]->GetGlobalScale();
    glm::vec3 wallsScl = walls->GetScale();

    if (projectilePos.x - projectileScl.x >= -wallsScl.x / 2.0 &&
        projectilePos.x + projectileScl.x <= wallsScl.x / 2.0 &&
        projectilePos.z - projectileScl.z >= -wallsScl.z / 2.0 &&
        projectilePos.z + projectileScl.z <= wallsScl.z / 2.0)
        return false;

    return true;
}

bool RenderObject3D::CollisionTest(TankProjectile* projectile, Building* building)
{
    glm::vec3 projectilePos = projectile->GetPosition();
    float projectileRadius = projectile->GetPrimitives()[0]->GetGlobalScale().x;

    glm::vec3 buildingPos = building->GetPrimitives()[0]->GetGlobalPosition();
    glm::vec3 buildingScl = building->GetPrimitives()[0]->GetGlobalScale();

    auto buildingMin = buildingPos - (buildingScl / glm::vec3(2.0, 2.0, 2.0));
    auto buildingMax = buildingPos + (buildingScl / glm::vec3(2.0, 2.0, 2.0));

    if ((projectilePos.x + projectileRadius >= buildingMin.x &&
         projectilePos.x + projectileRadius <= buildingMax.x && projectilePos.z >= buildingMin.z &&
         projectilePos.z <= buildingMax.z))
        return true;

    if ((projectilePos.x - projectileRadius >= buildingMin.x &&
         projectilePos.x - projectileRadius <= buildingMax.x && projectilePos.z >= buildingMin.z &&
         projectilePos.z <= buildingMax.z))
        return true;

    if ((projectilePos.z + projectileRadius >= buildingMin.z &&
         projectilePos.z + projectileRadius <= buildingMax.z && projectilePos.x >= buildingMin.x &&
         projectilePos.x <= buildingMax.x))
        return true;

    if ((projectilePos.z - projectileRadius >= buildingMin.z &&
         projectilePos.z - projectileRadius <= buildingMax.z && projectilePos.x >= buildingMin.x &&
         projectilePos.x <= buildingMax.x))
        return true;

    return false;
}

bool RenderObject3D::CollisionTest(TankProjectile* projectile, Tank* tank)
{
    glm::vec3 projectilePos = projectile->GetPosition();
    float projectileRadius = projectile->GetPrimitives()[0]->GetGlobalScale().x;

    glm::vec3 tankPos = tank->GetPosition();
    float tankRadius = tank->GetTankRadius();

    auto tankMin = tankPos - glm::vec3(tankRadius / 2, 0, tankRadius / 2);
    auto tankMax = tankPos + glm::vec3(tankRadius / 2, 0, tankRadius / 2);

    tankMin.y = 0;
    tankMax.y = 0;

    for (auto primitive : tank->GetPrimitives()) {
        float y = primitive->GetGlobalPosition().y * (primitive->GetGlobalScale().y / 2.0);

        if (tankMax.y < y)
            tankMax.y = y;
    }

    if ((projectilePos.x + projectileRadius >= tankMin.x &&
         projectilePos.x + projectileRadius <= tankMax.x) &&
        (projectilePos.y - projectileRadius >= tankMin.y &&
         projectilePos.y + projectileRadius <= tankMax.y) &&
        (projectilePos.z >= tankMin.z && projectilePos.z <= tankMax.z))
        return true;

    if ((projectilePos.x - projectileRadius >= tankMin.x &&
         projectilePos.x - projectileRadius <= tankMax.x) &&
        (projectilePos.y - projectileRadius >= tankMin.y &&
         projectilePos.y + projectileRadius <= tankMax.y) &&
        (projectilePos.z >= tankMin.z && projectilePos.z <= tankMax.z))
        return true;

    if ((projectilePos.x >= tankMin.x && projectilePos.x <= tankMax.x) &&
        (projectilePos.y - projectileRadius >= tankMin.y &&
         projectilePos.y + projectileRadius <= tankMax.y) &&
        (projectilePos.z + projectileRadius >= tankMin.z &&
         projectilePos.z + projectileRadius <= tankMax.z))
        return true;

    if ((projectilePos.x >= tankMin.x && projectilePos.x <= tankMax.x) &&
        (projectilePos.y - projectileRadius >= tankMin.y &&
         projectilePos.y + projectileRadius <= tankMax.y) &&
        (projectilePos.z - projectileRadius >= tankMin.z &&
         projectilePos.z - projectileRadius <= tankMax.z))
        return true;

    return false;
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
