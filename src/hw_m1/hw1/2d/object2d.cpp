#include "object2d.h"
#include "core/gpu/vertex_format.h"
#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "hw_m1/hw1/2d/render_object2d/render_objects2d.h"
#include "hw_m1/hw1/2d/render_object2d/square/square.h"
#include "hw_m1/hw1/2d/scene_object2d/scene_object2d.h"
#include <iostream>

using namespace m1;

Object2D::Object2D(Object2D& object2D)
    : RenderObject2D(object2D)
    , SceneObject2D(object2D)
{
}

Object2D::~Object2D()
{
    for (RenderObject2D* object : objects)
        delete object;
}

void Object2D::Update(float deltaTimeSeconds)
{
}

std::vector<std::pair<Mesh*, glm::mat3> > Object2D::GetRenderDetails()
{
    std::vector<std::pair<Mesh*, glm::mat3> > result;

    if (dirtyFlag)
        ComputeModelMatrix();

    for (RenderObject2D* object : objects) {
        if (dynamic_cast<Object2D*>(object) != nullptr)
            for (RenderObject2D* obj : dynamic_cast<Object2D*>(object)->objects)
                result.push_back({ obj->GetMesh(), obj->GetModelMatrix() });

        result.push_back({ object->GetMesh(), object->GetModelMatrix() });
    }

    return result;
}

ColorObject2D::ColorObject2D(glm::vec3 color)
    : color(color)
{
}

ColorObject2D::ColorObject2D(ColorObject2D& colorObject2D)
    : Object2D(colorObject2D)
    , color(colorObject2D.color)
{
}

glm::vec3 ColorObject2D::GetColor()
{
    return color;
}

Limit::Limit(Limit& l)
    : Limit()
{
    auto limit = l.objects[0]->Clone();

    objects.push_back(limit);
    AddChildren(limit);

    limit->Init();
}

Limit* Limit::Create()
{
    return new Limit();
}

Limit* Limit::Clone()
{
    return new Limit(*this);
}

void Limit::Init()
{
    Square* limit = new Square(VertexColor::RED, true);

    objects.push_back(limit);
    AddChildren(limit);

    limit->Init();
}

Projectile::Projectile()
    : Projectile(VertexColor::WHITE, 0)
{
}

Projectile::Projectile(glm::vec3(color), int life)
    : ColorObject2D(color)
    , life(life)
{
}

Projectile* Projectile::Create()
{
    return new Projectile();
}

Projectile* Projectile::Clone()
{
    return new Projectile(*this);
}

void Projectile::Init()
{
    Star* star = new Star(color);

    objects.push_back(star);
    AddChildren(star);

    star->Init();
}

Launcher::Launcher()
    : Launcher(VertexColor::WHITE, 0)
{
}

Launcher::Launcher(glm::vec3 color, int cost)
    : ColorObject2D(color)
    , cost(cost)
    , life(cost)
    , timeout(RESET_TIME)
{
}

Launcher::Launcher(Launcher& launcher)
    : ColorObject2D(launcher)
    , cost(launcher.cost)
    , life(launcher.cost)
    , timeout(RESET_TIME)
{
    auto* body = launcher.objects[0]->Clone();

    objects.push_back(body);
    AddChildren(body);

    auto* pipe = launcher.objects[1]->Clone();

    objects.push_back(pipe);
    body->AddChildren(pipe);

    for (auto* object : objects)
        object->Init();
}

Projectile* Launcher::Launch()
{
    Projectile* projectile = nullptr;

    if (timeout < RESET_TIME || !enabled)
        return projectile;

    timeout = 0.0;
    life--;

    projectile = new Projectile(color, cost);
    projectile->AttatchToParent(parent);
    projectile->DetatchFromParent();
    glm::vec2 scl = projectile->GetScale();
    float modifier = 0.4;
    projectile->SetScale({ scl.x * modifier, scl.y * modifier });
    projectile->Init();

    return projectile;
}

int Launcher::GetCost()
{
    return cost;
}

Launcher* Launcher::Create()
{
    return new Launcher();
}

Launcher* Launcher::Clone()
{
    return new Launcher(*this);
}

void Launcher::Update(float deltaTimeSeconds)
{
    if (life <= 0)
        enabled = false;

    timeout += deltaTimeSeconds;
}

void Launcher::Init()
{
    Diamond* body = new Diamond(color);

    body->SetScale({ 0.35f, 0.35f });
    body->SetPosition({ -0.25f, 0 });

    objects.push_back(body);
    AddChildren(body);

    Square* pipe = new Square(color, true);

    pipe->SetScale({ 1.15, 0.5 });
    pipe->SetPosition({ 0.8, 0 });

    objects.push_back(pipe);
    body->AddChildren(pipe);

    for (auto* object : objects)
        object->Init();
}

Spot::Spot()
    : launcher(nullptr)
{
}

Spot::Spot(Spot& spot)
    : Spot()
{
    launcher = spot.launcher->Clone();
}

Spot* Spot::Create()
{
    return new Spot();
}

Spot* Spot::Clone()
{
    return new Spot(*this);
}

void Spot::Attatch(Launcher* launcher)
{
    this->launcher = launcher;
}

void Spot::Detatch()
{
    delete launcher;
    launcher = nullptr;
}

Launcher* Spot::GetLauncher()
{
    return launcher;
}

void Spot::Init()
{
    Square* spot = new Square(VertexColor::GREEN, true);
    spot->Init();
    objects.push_back(spot);
    AddChildren(spot);
}

Generator::Generator()
    : Generator(VertexColor::WHITE, 0)
{
}

Generator::Generator(glm::vec3 color, int cost)
    : ColorObject2D(color)
    , launcher(nullptr)
    , cost(cost)
{
}

Generator::Generator(Generator& generator)
    : Generator(generator.color, generator.cost)
{
    launcher = generator.launcher->Clone();
}

Generator* Generator::Create()
{
    return new Generator();
}

Generator* Generator::Clone()
{
    return new Generator(*this);
}

Launcher* Generator::Generate()
{
    return new Launcher(*launcher);
}

int Generator::GetCost()
{
    return cost;
}

void Generator::Init()
{
    Square* frame = new Square(VertexColor::GRAY, false);

    objects.push_back(frame);
    AddChildren(frame);

    launcher = new Launcher(color, cost);

    objects.push_back(launcher);
    AddChildren(launcher);

    for (int i = 1; i <= cost; i++) {
        Star* energy = new Star(VertexColor::GRAY);

        energy->SetScale({ 0.25, 0.25 });
        energy->SetPosition({ -0.6 + (i * 0.3), -0.65 });

        objects.push_back(energy);
        AddChildren(energy);
    }

    for (auto* object : objects)
        object->Init();
}

Enemy::Enemy()
    : Enemy(VertexColor::WHITE, 0)
{
}

Enemy::Enemy(glm::vec3 color, int life)
    : ColorObject2D(color)
    , life(life)
{
}

Enemy::Enemy(Enemy& enemy)
    : Enemy(enemy.color, enemy.life)
{
}

Enemy* Enemy::Create()
{
    return new Enemy();
}

Enemy* Enemy::Clone()
{
    return new Enemy(*this);
}

void Enemy::Init()
{
    Hexagon* filler = new Hexagon(VertexColor::CYAN);
    Hexagon* outer = new Hexagon(color);

    filler->SetScale({ 0.7, 0.7 });

    objects = { filler, outer };

    AddChildren(filler);
    AddChildren(outer);

    for (auto* object : objects)
        object->Init();
}

Life* Life::Create()
{
    return new Life();
}

Life* Life::Clone()
{
    return new Life(*this);
}

void Life::Init()
{
    Square* life = new Square(VertexColor::RED, true);
    objects.push_back(life);
    AddChildren(life);

    life->Init();
}

Energy::Energy()
    : ColorObject2D(VertexColor::GRAY)
{
}

Energy::Energy(glm::vec3 color)
    : ColorObject2D(color)
{
}

Energy* Energy::Create()
{
    return new Energy();
}

Energy* Energy::Clone()
{
    return new Energy(*this);
}

void Energy::Init()
{
    Star* star = new Star(color);

    objects.push_back(star);
    AddChildren(star);

    star->Init();
}
