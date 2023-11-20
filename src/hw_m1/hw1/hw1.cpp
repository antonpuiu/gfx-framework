#include "hw_m1/hw1/hw1.h"

#include "GLFW/glfw3.h"
#include "core/gpu/vertex_format.h"
#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "hw_m1/hw1/2d/render_object2d/square/square.h"
#include "hw_m1/hw1/2d/scene_object2d/scene_object2d.h"
#include "utils/math_utils.h"
#include "hw_m1/hw1/2d/render_object2d/render_objects2d.h"

#include "hw_m1/hw1/2d/object2d.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Hw1::Hw1()
{
}

Hw1::~Hw1()
{
}

void Hw1::Init()
{
    {
        // Camera Setup
        glm::ivec2 resolution = window->GetResolution();
        auto camera = GetSceneCamera();
        camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
        camera->SetPosition(glm::vec3(0, 0, 50));
        camera->SetRotation(glm::vec3(0, 0, 0));
        camera->Update();
        GetCameraInput()->SetActive(false);
    }

    {
        // Limit and spots
        limit = new Limit();
        limit->SetPosition({ 35, 170 });
        limit->SetScale({ 50, 320 });
        objects.push_back(limit);

        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                Spot* spot = new Spot();

                spot->SetPosition({ 110 * i + 10, 110 * j - 50 });
                spot->SetScale({ 100, 100 });

                objects.push_back(spot);
                spots.push_back(spot);
            }
        }
    }

    {
        // Generators and and Enemies [ one of each kind ]
        std::unordered_map<glm::vec3, int> colors{ { VertexColor::PURPLE, 3 },
                                                   { VertexColor::YELLOW, 2 },
                                                   { VertexColor::BLUE, 2 },
                                                   { VertexColor::ORANGE, 1 } };

        std::vector<float> heights;
        int current = 1;

        for (auto* spot : spots) {
            float y = spot->GetPosition().y;

            bool found = false;

            for (float height : heights) {
                if (height == y) {
                    found = true;
                    break;
                }
            }

            if (!found)
                heights.push_back(y);
        }

        for (auto [color, cnt] : colors) {
            Enemy* enemy = new Enemy(color);

            enemy->SetScale({ 80, 80 });
            // enemy->SetPosition({ 1280 + (current * 100), heights[current % heights.size()] });
            enemy->SetPosition({ 800 + (current * 100), heights[current % heights.size()] });

            objects.push_front(enemy);
            enemies.push_back(enemy);

            current++;
        }

        current = 1;

        for (auto [color, cnt] : colors) {
            Generator* generator = new Generator(color, cnt);

            generator->SetScale({ 100, 100 });
            generator->SetPosition({ -30 + (current * 110), 650 });

            objects.push_front(generator);
            generators.push_back(generator);

            current++;
        }
    }

    {
        // Lifes & Power
        for (int i = 1; i <= 3; i++) {
            Life* life = new Life();
            life->SetPosition({ 850 + (110 * i), 650 });
            life->SetScale({ 100, 100 });
            objects.push_back(life);
            lifes.push_back(life);
        }

        for (int i = 1; i <= 13; i++) {
            Energy* energy = new Energy();
            energy->SetPosition({ 895 + (25 * i), 580 });
            energy->SetScale({ 20, 20 });
            objects.push_back(energy);
            currentEnergy.push_back(energy);
        }

        for (int i = 1; i <= 4; i++) {
            Energy* energy = new Energy(VertexColor::PURPLE);

            energy->enabled = false;
            energy->SetScale({ 60, 60 });

            objects.push_front(energy);
            newEnergy.push_back(energy);
        }
    }

    for (auto* object : objects)
        object->Init();

    EnableNewEnergy(3);
}

void Hw1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Hw1::Update(float deltaTimeSeconds)
{
    for (auto enemy : enemies) {
        if (!enemy->enabled)
            continue;

        switch (enemy->state) {
        case ACTIVE:
            enemy->AddPosition({ -300 * deltaTimeSeconds, 0 });
            enemy->AddRotation(90 * deltaTimeSeconds * TO_RADIANS);
            break;
        case HIT:
            enemy->AddScale({ -300 * deltaTimeSeconds, -300 * deltaTimeSeconds });
            break;
        case DISABLED:
            break;
        }
    }

    for (auto* object : objects) {
        if (!object->enabled)
            continue;

        for (auto [mesh, modelMatrix] : object->GetRenderDetails())
            RenderMesh2D(mesh, shaders["VertexColor"], modelMatrix);
    }
}

void Hw1::FrameEnd()
{
    for (auto enemy : enemies) {
        if (enemy->GetPosition().x <= limit->GetPosition().x) {
            enemy->state = HIT;
        }

        if (enemy->GetScale().x < 50 || enemy->GetScale().y < 50) {
            enemy->state = DISABLED;
            enemy->enabled = false;
        }
    }

    // int disabled = 0;

    // for (auto* enemy : enemies)
    //     if (!enemy->enabled)
    //         disabled++;

    // if (disabled == enemies.size()) {
    //     EnableNewEnergy(3);
    //     EnableEnemies(3);
    // }

    int count = 0;

    for (auto energy : newEnergy)
        if (energy->enabled)
            count++;

    if (count == 0)
        EnableNewEnergy(3);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Hw1::OnInputUpdate(float deltaTime, int mods)
{
}

void Hw1::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Hw1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Hw1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseY = abs(window->GetResolution().y - mouseY);

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && launcher != nullptr)
        launcher->SetPosition({ mouseX, mouseY });
}

void Hw1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = abs(window->GetResolution().y - mouseY);

    if (button == 1) {
        for (auto* energy : newEnergy) {
            if (!energy->enabled)
                continue;

            auto pos = energy->GetPosition();
            auto scl = energy->GetScale();

            if ((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
                (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2))
                energy->enabled = false;
        }
    }

    if (button == 1) {
        for (auto generator : generators) {
            auto pos = generator->GetPosition();
            auto scl = generator->GetScale();

            if ((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
                (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2) &&
                generator->GetCost() <= GetCurrentEnergyLevel()) {
                launcher = generator->Generate();
                launcher->DetatchFromParent();

                objects.push_front(launcher);

                int cost = generator->GetCost();

                for (int i = currentEnergy.size() - 1; i >= 0; i--) {
                    if (cost == 0)
                        break;

                    if (!currentEnergy[i]->enabled)
                        continue;

                    currentEnergy[i]->enabled = false;
                    cost--;
                }

                break;
            }
        }
    }
}

void Hw1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    mouseY = abs(window->GetResolution().y - mouseY);

    for (auto spot : spots) {
        auto pos = spot->GetPosition();
        auto scl = spot->GetScale();

        if ((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
            (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2)) {
            if (launcher != nullptr && button == 1) {
                spot->Attatch(launcher);
                launcher->AttatchToParent(spot);
                launcher = nullptr;
                return;
            }

            if (button == 2)
                spot->Detatch();
        }
    }

    if (launcher != nullptr) {
        for (auto spot : spots) {
            auto pos = spot->GetPosition();
            auto scl = spot->GetScale();

            if ((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
                (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2)) {
                spot->Attatch(launcher);
                launcher->AttatchToParent(spot);
                launcher = nullptr;
                return;
            }
        }

        launcher->AttatchToParent();
    }
}

void Hw1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Hw1::OnWindowResize(int width, int height)
{
}

void Hw1::EnableNewEnergy(int count)
{
    std::random_device rd;
    std::default_random_engine engine(rd());
    glm::ivec2 resolution = window->GetResolution();
    std::uniform_int_distribution<int> x(60, resolution.x), y(60, resolution.y);

    for (int i = 0; i < count; i++) {
        if (i == newEnergy.size())
            break;

        newEnergy[i]->enabled = true;
        newEnergy[i]->SetPosition({ x(engine), y(engine) });
    }
}

void Hw1::EnableEnemies(int count)
{
    for (int i = 0; i < count; i++) {
        if (i == enemies.size())
            break;

        enemies[i]->Reset();
        enemies[i]->SetPosition({ 1280 + (i * 100), enemies[i]->GetPosition().y });
        enemies[i]->SetScale({ 80, 80 });
    }
}

int Hw1::GetCurrentEnergyLevel()
{
    int result = 0;

    for (auto energy : currentEnergy)
        if (energy->enabled)
            result++;

    return result;
}
