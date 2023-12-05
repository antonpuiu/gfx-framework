#include "hw_m1/hw2/3d/object3d.h"
#include "core/gpu/vertex_format.h"
#include "glm/gtx/quaternion.hpp"
#include "hw_m1/hw2/3d/render_object3d.h"
#include "hw_m1/hw2/3d/transform3d.h"
#include "lab_m1/lab5/lab_camera.h"
#include "utils/math_utils.h"
#include <iostream>

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

Building::Building()
    : hp(30)
{
}

TankProjectile::TankProjectile(Tank* tank)
    : tank(tank)
{
}

Tank::Tank()
    : Tank(nullptr)
{
}

Tank::Tank(implemented::Camera* camera)
    : Object3D()
    , camera(camera)
    , hp(100)
    , timeout(0.0)
{
    Tank::instances.push_back(this);
}

Tank::~Tank()
{
    for (auto it = Tank::instances.begin(); it != Tank::instances.end(); it++) {
        if (*it == this) {
            Tank::instances.erase(it);
            break;
        }
    }
}

Object3D::~Object3D()
{
    for (auto object : objects)
        delete object;

    for (auto primitive : primitives)
        delete primitive;
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
    if (!(objects.empty() && primitives.empty()))
        return;

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
    cube->SetColor(VertexColor::GREEN + glm::vec3(0.7, 0, 0.7));
    primitives.push_back(cube);
}

void Walls::InitObject()
{
    Cube* cube = new Cube();
    cube->SetColor(VertexColor::GRAY);
    primitives.push_back(cube);
}

void Cylinder::InitObject()
{
    CylinderCover* topCover = new CylinderCover();
    CylinderCover* bottomCover = new CylinderCover();
    CylinderBody* body = new CylinderBody();

    topCover->SetPosition({ 0, 1, 0 });

    primitives = { topCover, bottomCover, body };
}

void Building::InitObject()
{
    Cube* cube = new Cube();
    cube->SetColor(VertexColor::colors[colors(engine)]);
    cube->SetScale({ width(engine), height(engine), length(engine) });
    cube->SetPosition({ 0, cube->GetScale().y / 2, 0 });
    primitives.push_back(cube);
}

void TankBody::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.5, 0.2, 0.75 });
    cube->SetColor(VertexColor::GREEN - glm::vec3(0, 0.7, 0));
    primitives.push_back(cube);
}

void TankTurret::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.3, 0.1, 0.3 });
    cube->SetColor(VertexColor::GREEN - glm::vec3(0, 0.5, 0));
    primitives.push_back(cube);

    gun = new TankGun();
    gun->SetScale({ 0.1, 0.1, 0.1 });
    gun->SetRotation({ 90, 0, 0 });
    objects.push_back(gun);
}

bool TankProjectile::IsParent(Tank* tank)
{
    return tank == this->tank;
}

void TankProjectile::Update(float deltaTimeSeconds)
{
    switch (state) {
    case ACTIVE:
    case LAUNCHED:
        timeToLive -= deltaTimeSeconds;
        AddPosition({ 10 * deltaTimeSeconds * sin(GetRotation().y * TO_RADIANS),
                      10 * deltaTimeSeconds * cos(GetRotation().x * TO_RADIANS),
                      10 * deltaTimeSeconds * cos(GetRotation().y * TO_RADIANS) });
        break;
    case STOPPING:
    case INACTIVE:
        break;
    }

    if (timeToLive <= 0)
        state = INACTIVE;
}

void TankProjectile::InitObject()
{
    Sphere* sphere = new Sphere();

    sphere->SetScale({ 0.02, 0.02, 0.02 });
    sphere->SetColor(VertexColor::BLACK);

    primitives.push_back(sphere);

    timeToLive = 2;
}

void TankGun::InitObject()
{
    Cylinder* cylinder = new Cylinder();

    cylinder->SetScale({ 0.2, 6, 0.2 });

    objects = { cylinder };
}

void TankTracks::InitObject()
{
    Cube* cube = new Cube();
    cube->SetScale({ 0.1, 0.1, 0.8 });
    primitives.push_back(cube);
}

void Tank::RotateTurret(float deg)
{
    turret->AddRotation({ 0, deg, 0 });
    UpdateCamera();
}

void Tank::RotateGun(float deg)
{
    float crt_rot = turret->gun->GetRotation().x + deg;

    if (crt_rot > 70 && crt_rot < 94)
        turret->gun->AddRotation({ deg, 0, 0 });
}

glm::vec3 Tank::GetGunRot()
{
    return turret->gun->GetRotation();
}

float Tank::GetTankRadius()
{
    Cube* body = static_cast<Cube*>(objects[2]->GetPrimitives()[0]);
    glm::vec3 bodyPos = body->GetGlobalPosition();
    glm::vec3 bodyScl = body->GetGlobalScale();

    return bodyPos.z + bodyScl.z / 2 - pos.z;
}

float Building::GetHp()
{
    return hp;
}

float Tank::GetTankHP()
{
    return hp;
}

void Building::Strike()
{
    hp -= 10;
}

void Tank::Strike()
{
    hp -= 15;
}

TankProjectile* Tank::LaunchProjectile()
{
    if (timeout > 0.0)
        return nullptr;

    TankProjectile* projectile = new TankProjectile(this);

    projectile->AttatchToParent(turret->gun);
    projectile->DetatchFromParent();
    projectile->state = LAUNCHED;

    timeout = 1.0;

    return projectile;
}

void Tank::AddPosition(glm::vec3 qty)
{
    Transform3D::AddPosition(qty);
    UpdateCamera();
}

void Tank::AddRotation(glm::vec3 qty)
{
    Transform3D::AddRotation(qty);
    UpdateCamera();
}

void Building::Update(float deltaTimeSeconds)
{
    if (hp <= 0)
        state = INACTIVE;
}

void Tank::Update(float deltaTimeSeconds)
{
    if (hp <= 0)
        state = INACTIVE;

    if (timeout > 0)
        timeout -= deltaTimeSeconds;

    switch (state) {
    case ACTIVE:
    case LAUNCHED:
    case STOPPING:
    case INACTIVE:
        break;
    }
}

void Tank::InitObject()
{
    TankTracks *left = new TankTracks(), *right = new TankTracks();
    TankBody* body = new TankBody();
    turret = new TankTurret();

    turret->Init();

    left->SetPosition({ -0.16, 0, 0 });
    right->SetPosition({ 0.16, 0, 0 });
    body->SetPosition({ 0, 0.1, 0 });
    turret->SetPosition({ 0, 0.25, 0 });
    AddPosition({ 0, 0.1, 0 });

    objects = { left, right, body, turret };
    UpdateCamera();
}

void Tank::UpdateCamera()
{
    if (camera == nullptr)
        return;

    camera->Set(pos + glm::vec3(0, 0.45, -0.5), pos, glm::vec3(0, 1, 0));
    camera->distanceToTarget = abs(glm::distance(camera->position, pos));
    camera->RotateThirdPerson_OY((rotation.y + turret->rotation.y) * TO_RADIANS);
    camera->RotateThirdPerson_OX(-(turret->gun->GetRotation().x - 90) * TO_RADIANS);
    camera->RotateFirstPerson_OX(30 * TO_RADIANS);
}
