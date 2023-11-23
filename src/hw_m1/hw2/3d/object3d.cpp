#include "hw_m1/hw2/3d/object3d.h"

using namespace m1;

Object3D::Object3D()
    : RenderObject3D()
    , SceneObject3D()
{
}

Object3D::Object3D(Object3D& object3D)
    : RenderObject3D(object3D)
    , SceneObject3D(object3D)
{
}

Object3D::~Object3D()
{
    for (auto primitive : primitives)
        delete primitive;

    for (auto object : objects)
        delete object;
}

std::vector<RenderObject3D*> Object3D::GetPrimitives()
{
    std::vector<RenderObject3D*> result;

    if (dirtyFlag)
        ComputeModelMatrix();

    for (auto object : objects) {
        auto renderObjects = object->GetPrimitives();
        result.insert(result.end(), renderObjects.begin(), renderObjects.end());
    }

    for (auto primitive : primitives)
        result.push_back(primitive);

    return result;
}

void Object3D::Update(float deltaTimeSeconds)
{
}

void Object3D::Init()
{
    InitObject();

    for (auto object : objects) {
        object->Init();
        AddChildren(object);
    }

    for (auto primitive : primitives) {
        primitive->Init();
        AddChildren(primitive);
    }
}

void Plane::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 1, 0, 1 });
    cube->SetColor(VertexColor::GRAY);
    primitives.push_back(cube);
}

void TankBody::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.4, 0.2, 0.7 });
    cube->SetPosition({ 0, 0.15, 0 });
    cube->SetColor(VertexColor::GREEN - glm::vec3(0, 0.9, 0));
    primitives.push_back(cube);
}

void TankTurret::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.3, 0.1, 0.3 });
    cube->SetPosition({ 0, 0.3, 0 });
    cube->SetColor(VertexColor::GREEN - glm::vec3(0, 0.7, 0));
    primitives.push_back(cube);
}

void TankProjectile::InitObject()
{
    Sphere* sphere = new Sphere();

    primitives.push_back(sphere);
}

void TankGun::InitObject()
{
    CylinderCover* topCover = new CylinderCover();
    CylinderCover* bottomCover = new CylinderCover();
    CylinderBody* body = new CylinderBody();

    topCover->SetPosition({ 0, 1, 0 });

    primitives.push_back(topCover);
    primitives.push_back(bottomCover);
    primitives.push_back(body);
}

void TankTracks::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.1, 0.1, 0.6 });
    primitives.push_back(cube);
}

void Tank::InitObject()
{
    // objects.push_back(new TankGun());
    objects.push_back(new TankProjectile());
}
