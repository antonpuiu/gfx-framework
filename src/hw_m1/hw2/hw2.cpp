#include "hw2.h"

#include "GLFW/glfw3.h"
#include "core/gpu/mesh.h"
#include "core/gpu/vertex_format.h"
#include "hw_m1/hw2/3d/object3d.h"
#include "hw_m1/hw2/3d/render_object3d.h"
#include "hw_m1/hw2/3d/scene_object3d.h"
#include "lab_m1/lab1/lab1.h"

#include "utils/math_utils.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Hw2::Hw2()
{
}

Hw2::~Hw2()
{
}

void Hw2::Init()
{
    {
        camera = new implemented::Camera();
        camera->Set(glm::vec3(0, 1, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }

    {
        Shader* shader = new Shader("HwShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HW_M1, "hw2", "shaders",
                                    "VertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HW_M1, "hw2", "shaders",
                                    "FragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("HwTankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HW_M1, "hw2", "shaders",
                                    "TankVertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::HW_M1, "hw2", "shaders",
                                    "TankFragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Plane* plane = new Plane();
        walls = new Walls();

        plane->SetScale({ 90, 0, 90 });
        plane->SetPosition({ 0, 0, 0 });

        walls->SetScale({ 90, 90, 90 });
        walls->SetPosition({ 0, 45, 0 });

        objects.push_back(plane);
        objects.push_back(walls);
    }

    {
        tank = new Tank(camera);
        objects.push_back(tank);

        int count = Tank::count(Tank::engine);

        for (int i = 0; i < count; i++) {
            Tank* enemy = new Tank();

            enemy->SetPosition({ Tank::position(Tank::engine), 0, Tank::position(Tank::engine) });
            enemy->SetRotation({ 0, Tank::rot(Tank::engine), 0 });
            enemy->Init();

            enemies.push_back(enemy);
        }
    }

    {
        int count = Building::count(Building::engine);

        for (int i = 0; i < count; i++) {
            Building* building = new Building();
            building->SetPosition(
                { Building::position(Building::engine), 0, Building::position(Building::engine) });

            objects.push_back(building);
            buildings.push_back(building);
        }
    }

    {
        for (auto object : objects)
            object->Init();
    }

    {
        timeLeft = Hw2::count(Hw2::engine) * 60.0;
        cout << "TimeLeft: " << timeLeft << endl;
    }
}

void Hw2::FrameStart()
{
    {
        glm::ivec2 resolution = window->props.resolution;
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, resolution.x, resolution.y);
    }

    {
        for (auto projectile : projectiles) {
            for (auto enemy : enemies) {
                if (RenderObject3D::CollisionTest(projectile, enemy)) {
                    projectile->state = INACTIVE;
                    enemy->Strike();
                }
            }

            if (RenderObject3D::CollisionTest(projectile, walls))
                projectile->state = INACTIVE;

            for (auto building : buildings)
                if (RenderObject3D::CollisionTest(projectile, building))
                    projectile->state = INACTIVE;
        }
    }

    {
        for (auto it_p = projectiles.begin(); it_p != projectiles.end();) {
            auto projectile = *it_p;
            bool erased = false;

            if (projectile->state != INACTIVE) {
                it_p++;
                continue;
            }

            for (auto it_o = objects.begin(); it_o != objects.end(); it_o++) {
                auto object = *it_o;

                if (projectile == object) {
                    objects.erase(it_o);
                    it_p = projectiles.erase(it_p);
                    erased = true;
                    delete projectile;
                    break;
                }
            }

            if (!erased)
                it_p++;
        }
    }

    for (auto it_e = enemies.begin(); it_e != enemies.end();) {
        auto enemy = *it_e;

        if (enemy->state != INACTIVE) {
            it_e++;
            continue;
        }

        it_e = enemies.erase(it_e);
        delete enemy;
    }

    {
        if (timeLeft <= 0) {
            cout << "Game over!" << endl;
            window->Close();
        }
    }
}

void Hw2::Update(float deltaTimeSeconds)
{
    {
        tank->Update(deltaTimeSeconds);
    }

    {
        for (auto projectile : projectiles)
            projectile->Update(deltaTimeSeconds);
    }

    {
        for (auto enemy : enemies) {
            enemy->Update(deltaTimeSeconds);

            bool moveBack = false;

            enemy->AddPosition({ deltaTimeSeconds * sin(enemy->GetRotation().y * TO_RADIANS), 0,
                                 deltaTimeSeconds * cos(enemy->GetRotation().y * TO_RADIANS) });

            for (auto building : buildings) {
                if (RenderObject3D::CollisionTest(enemy, building)) {
                    moveBack = true;
                    break;
                }
            }

            if (RenderObject3D::CollisionTest(tank, enemy) ||
                RenderObject3D::CollisionTest(enemy, walls) || moveBack) {
                enemy->AddPosition(
                    { -deltaTimeSeconds * sin(enemy->GetRotation().y * TO_RADIANS), 0,
                      -deltaTimeSeconds * cos(enemy->GetRotation().y * TO_RADIANS) });
                if (enemy->GetRotation().y > 0)
                    enemy->AddRotation({ 0, 500 * deltaTimeSeconds, 0 });
                else
                    enemy->AddRotation({ 0, 500 * -deltaTimeSeconds, 0 });
            }
        }
    }

    {
        timeLeft -= deltaTimeSeconds;
    }
}

void Hw2::FrameEnd()
{
    for (auto object : objects)
        for (auto primitive : object->GetPrimitives())
            RenderSimpleMesh(primitive->GetMesh(), shaders["HwShader"], primitive->GetModelMatrix(),
                             primitive->GetColor());

    for (auto enemy : enemies)
        for (auto primitive : enemy->GetPrimitives())
            RenderEnemyMesh(primitive->GetMesh(), shaders["HwTankShader"],
                            primitive->GetModelMatrix(), primitive->GetColor(), enemy->GetTankHP());
}

void Hw2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
                           glm::vec3 color)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    {
        int modelLocation = glGetUniformLocation(shader->program, "Model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        int viewLocation = glGetUniformLocation(shader->program, "View");
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        int projLocation = glGetUniformLocation(shader->program, "Projection");
        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        int colorLocation = glGetUniformLocation(shader->program, "object_color");
        glUniform3fv(colorLocation, 1, glm::value_ptr(color));
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Hw2::RenderEnemyMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color,
                          float hp)
{
    if (!mesh || !shader || !shader->program)
        return;

    glUseProgram(shader->program);

    {
        int modelLocation = glGetUniformLocation(shader->program, "Model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        int viewLocation = glGetUniformLocation(shader->program, "View");
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        int projLocation = glGetUniformLocation(shader->program, "Projection");
        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        int colorLocation = glGetUniformLocation(shader->program, "object_color");
        glUniform3fv(colorLocation, 1, glm::value_ptr(color));

        int hpLocation = glGetUniformLocation(shader->program, "hp");
        glUniform1f(hpLocation, hp);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Hw2::OnInputUpdate(float deltaTime, int mods)
{
    {
        float sensitivityOX = 10.0;
        float sensitivityOY = 80.0;
        float boost = 50.0;

        {
            // Body
            if (window->KeyHold(GLFW_KEY_W)) {
                bool moveBack = false;

                if ((mods & GLFW_MOD_SHIFT) != 0) {
                    tank->AddPosition(
                        { boost * deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                          boost * deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });

                    for (auto building : buildings) {
                        if (RenderObject3D::CollisionTest(tank, building)) {
                            moveBack = true;
                            break;
                        }
                    }

                    for (auto enemy : enemies) {
                        if (RenderObject3D::CollisionTest(tank, enemy)) {
                            moveBack = true;
                            break;
                        }
                    }

                    if (RenderObject3D::CollisionTest(tank, walls) || moveBack)
                        tank->AddPosition(
                            { -boost * deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                              -boost * deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });
                } else {
                    bool moveBack = false;

                    tank->AddPosition({ 10 * deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                                        10 * deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });

                    for (auto building : buildings) {
                        if (RenderObject3D::CollisionTest(tank, building)) {
                            moveBack = true;
                            break;
                        }
                    }

                    for (auto enemy : enemies) {
                        if (RenderObject3D::CollisionTest(tank, enemy)) {
                            moveBack = true;
                            break;
                        }
                    }

                    if (RenderObject3D::CollisionTest(tank, walls) || moveBack)
                        tank->AddPosition(
                            { -10 * deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                              -10 * deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });
                }
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                bool moveBack = false;

                if ((mods & GLFW_MOD_SHIFT) != 0) {
                    tank->AddPosition(
                        { boost * -deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                          boost * -deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });

                    for (auto building : buildings) {
                        if (RenderObject3D::CollisionTest(tank, building)) {
                            moveBack = true;
                            break;
                        }
                    }

                    for (auto enemy : enemies) {
                        if (RenderObject3D::CollisionTest(tank, enemy)) {
                            moveBack = true;
                            break;
                        }
                    }

                    if (RenderObject3D::CollisionTest(tank, walls) || moveBack)
                        tank->AddPosition(
                            { -boost * -deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                              -boost * -deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });
                } else {
                    tank->AddPosition(
                        { 10 * -deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                          10 * -deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });

                    for (auto building : buildings) {
                        if (RenderObject3D::CollisionTest(tank, building)) {
                            moveBack = true;
                            break;
                        }
                    }

                    for (auto enemy : enemies) {
                        if (RenderObject3D::CollisionTest(tank, enemy)) {
                            moveBack = true;
                            break;
                        }
                    }

                    if (RenderObject3D::CollisionTest(tank, walls) || moveBack)
                        tank->AddPosition(
                            { -10 * -deltaTime * sin(tank->GetRotation().y * TO_RADIANS), 0,
                              -10 * -deltaTime * cos(tank->GetRotation().y * TO_RADIANS) });
                }
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                if ((mods & GLFW_MOD_SHIFT) != 0)
                    tank->AddRotation({ 0, 3 * sensitivityOY * deltaTime, 0 });
                else
                    tank->AddRotation({ 0, 3 * sensitivityOY * deltaTime, 0 });
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                if ((mods & GLFW_MOD_SHIFT) != 0)
                    tank->AddRotation({ 0, 3 * -sensitivityOY * deltaTime, 0 });
                else
                    tank->AddRotation({ 0, 3 * -sensitivityOY * deltaTime, 0 });
            }
        }
    }
}

void Hw2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_V) {
        cout << tank->GetGunRot() << endl;
    }

    if (key == GLFW_KEY_R) {
        cout << "Restart Game" << endl;

        {
            for (Building* building : buildings) {
                for (auto it = objects.begin(); it != objects.end(); it++) {
                    if (building == *it) {
                        objects.erase(it);
                        break;
                    }
                }

                delete building;
            }

            buildings.clear();
        }

        {
            int count = Building::count(Building::engine);

            for (int i = 0; i < count; i++) {
                Building* building = new Building();
                building->Init();
                building->SetPosition({ Building::position(Building::engine), 0,
                                        Building::position(Building::engine) });

                objects.push_back(building);
                buildings.push_back(building);
            }
        }
    }
}

void Hw2::OnKeyRelease(int key, int mods)
{
}

void Hw2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    {
        float sensivityOX = 0.15f;
        float sensivityOY = 0.15f;

        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
            tank->RotateGun(sensivityOX * deltaY);
            tank->RotateTurret(-sensivityOY * deltaX);
        }
    }
}

void Hw2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Hw2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    {
        if (button == 1) {
            TankProjectile* projectile = tank->LaunchProjectile();

            if (projectile == nullptr)
                return;

            projectile->Init();
            objects.push_back(projectile);
            projectiles.push_back(projectile);
        }
    }
}

void Hw2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Hw2::OnWindowResize(int width, int height)
{
}
