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
    : generateEnergy(0.0)
    , intervalEnergy(5.0)
    , generateEnemy(0.0)
    , intervalEnemy(0.0)
    , launcher(nullptr)
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
    }

    for (auto* object : objects)
        object->Init();
}

void Hw1::FrameStart()
{
    {
        // Clears the color buffer (using the previously set color) and depth buffer
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::ivec2 resolution = window->GetResolution();
        // Sets the screen area where to draw
        glViewport(0, 0, resolution.x, resolution.y);
    }

    {
        int lifesCount = 0;

        for (int i = 0; i < lifes.size(); i++) {
            if (!lifes[i]->enabled)
                break;

            lifesCount++;
        }

        if (lifesCount == 0) {
            cout << "GAME OVER!" << endl;
            window->Close();
        }
    }
}

void Hw1::Update(float deltaTimeSeconds)
{
    UpdateEnemies(deltaTimeSeconds);
    UpdateProjectiles(deltaTimeSeconds);
    UpdateSpots(deltaTimeSeconds);
    UpdateObjects(deltaTimeSeconds);

    generateEnergy += deltaTimeSeconds;
    generateEnemy += deltaTimeSeconds;
}

void Hw1::FrameEnd()
{
    {
        for (auto spot : spots) {
            Launcher* l = spot->GetLauncher();

            if (l == nullptr)
                continue;

            if (l->GetScale().x < 0.4 && l->GetScale().y < 0.4) {
                l->state = DISABLED;
                continue;
            }

            for (auto enemy : enemies) {
                if (enemy->GetColor() == l->GetColor() &&
                    enemy->GetPosition().y == spot->GetPosition().y &&
                    (spot->GetPosition().x + l->GetScale().x) -
                            (enemy->GetPosition().x - enemy->GetScale().x) >=
                        0) {
                    l->state = HIT;
                    break;
                }
            }
        }
    }

    {
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            Projectile* projectile = *it;
            glm::vec2 p_pos = projectile->GetPosition(), p_scl = projectile->GetScale();
            bool inc = true;

            switch (projectile->state) {
            case ENABLED:
                for (auto enemy : enemies) {
                    glm::vec2 e_pos = enemy->GetPosition(), e_scl = enemy->GetScale();

                    if (projectile->GetColor() == enemy->GetColor() && p_pos.y == e_pos.y &&
                        (p_pos.x + p_scl.x) - (e_pos.x - e_scl.x) >= 0) {
                        projectile->state = HIT;
                        enemy->life--;
                    }
                }
                break;
            case HIT:
                if (p_scl.x <= 0.4 && p_scl.y <= 0.4)
                    projectile->state = DISABLED;

                break;
            case DISABLED:
                for (auto it_obj = objects.begin(); it_obj != objects.end(); it_obj++) {
                    Object2D* obj = *it_obj;

                    if (obj == projectile) {
                        objects.erase(it_obj);
                        it = projectiles.erase(it);
                        inc = false;
                        break;
                    }
                }
                break;
            }

            if (inc)
                it++;
        }
    }

    {
        for (auto it = enemies.begin(); it != enemies.end();) {
            Enemy* enemy = *it;
            bool inc = true;

            switch (enemy->state) {
            case ENABLED:
                if (enemy->life <= 0)
                    enemy->state = HIT;

                if (enemy->GetPosition().x <= limit->GetPosition().x) {
                    enemy->state = HIT;

                    for (int i = lifes.size() - 1; i >= 0; i--) {
                        if (!lifes[i]->enabled)
                            continue;

                        lifes[i]->enabled = false;
                        break;
                    }
                }
                break;
            case HIT:
                if (enemy->GetScale().x < 50 || enemy->GetScale().y < 50) {
                    enemy->state = DISABLED;
                    enemy->enabled = false;
                }
                break;
            case DISABLED:
                for (auto it_obj = objects.begin(); it_obj != objects.end(); it_obj++) {
                    Object2D* obj = *it_obj;

                    if (obj == enemy) {
                        objects.erase(it_obj);
                        enemies.erase(it);
                        inc = false;
                        break;
                    }
                }
                break;
            }

            if (inc)
                it++;
        }
    }

    {
        glm::ivec2 resolution = window->GetResolution();

        for (auto it_proj = projectiles.begin(); it_proj != projectiles.end();) {
            Projectile* projectile = *it_proj;
            bool inc = true;

            if (!projectile->enabled || projectile->GetPosition().x > resolution.x) {
                for (auto it = objects.begin(); it != objects.end(); it++) {
                    Object2D* object = *it;

                    if (object == projectile) {
                        it = objects.erase(it);
                        it_proj = projectiles.erase(it_proj);
                        inc = false;
                    }
                }
            }

            if (inc)
                it_proj++;
        }
    }

    {
        for (auto spot : spots) {
            Launcher* l = spot->GetLauncher();

            if (l != nullptr && !l->enabled)
                spot->Detatch();
        }
    }

    {
        if (generateEnergy >= intervalEnergy && newEnergy.size() == 0) {
            std::random_device rd;
            std::default_random_engine engine(rd());
            std::uniform_real_distribution<float> x(1, 5), y(1, 5);

            generateEnergy = 0.0;
            intervalEnergy = x(engine);

            EnableNewEnergy(y(engine));
        }
    }

    {
        if (generateEnemy >= intervalEnemy) {
            std::random_device rd;
            std::default_random_engine engine(rd());
            std::uniform_real_distribution<float> x(1, 10);
            std::uniform_int_distribution<int> y(1, 2);

            generateEnemy = 0.0;
            intervalEnemy = x(engine);

            EnableEnemies(y(engine));
        }
    }
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
}

void Hw1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Hw1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseY = abs(window->GetResolution().y - mouseY);

    {
        if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && launcher != nullptr)
            launcher->SetPosition({ mouseX, mouseY });
    }
}

void Hw1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = abs(window->GetResolution().y - mouseY);

    {
        if (button == 1) {
            for (auto it = newEnergy.begin(); it != newEnergy.end();) {
                Energy* energy = *it;
                bool inc = true;

                auto pos = energy->GetPosition();
                auto scl = energy->GetScale();

                if ((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
                    (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2)) {
                    energy->enabled = false;
                    for (auto* crtEnergy : currentEnergy)
                        if (!crtEnergy->enabled) {
                            crtEnergy->enabled = true;
                            break;
                        }

                    for (auto it_obj = objects.begin(); it_obj != objects.end(); it_obj++) {
                        Object2D* obj = *it_obj;

                        if (obj == energy) {
                            it = newEnergy.erase(it);
                            objects.erase(it_obj);
                            inc = false;
                            break;
                        }
                    }
                }

                if (inc)
                    it++;
            }
        }
    }

    {
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
                    break;
                }
            }
        }
    }

    {
        if (button == 2) {
            for (Spot* spot : spots) {
                auto pos = spot->GetPosition();
                auto scl = spot->GetScale();

                if (!((pos.x - scl.x / 2 <= mouseX && mouseX <= pos.x + scl.x / 2) &&
                      (pos.y - scl.y / 2 <= mouseY && mouseY <= pos.y + scl.y / 2)))
                    continue;

                Launcher* l = spot->GetLauncher();

                if (l != nullptr) {
                    for (auto it = objects.begin(); it != objects.end(); it++)
                        if (*it == l) {
                            spot->Detatch();
                            objects.erase(it);
                            return;
                        }
                }
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

                int cost = launcher->GetCost();

                for (int i = currentEnergy.size() - 1; i >= 0; i--) {
                    if (cost == 0)
                        break;

                    if (!currentEnergy[i]->enabled)
                        continue;

                    currentEnergy[i]->enabled = false;
                    cost--;
                }

                launcher = nullptr;
                return;
            }
        }
    }

    if (launcher != nullptr && button == 1) {
        for (auto it = objects.begin(); it != objects.end(); it++) {
            if (*it == launcher) {
                objects.erase(it);
                delete launcher;
                break;
            }
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
        Energy* energy = new Energy(VertexColor::PURPLE);

        energy->Init();
        energy->SetScale({ 60, 60 });
        energy->SetPosition({ x(engine), y(engine) });

        objects.push_front(energy);
        newEnergy.push_back(energy);
    }
}

void Hw1::EnableEnemies(int count)
{
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

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> c(0, colors.size() - 1), h(0, heights.size() - 1);

    for (int i = 0; i < count; i++) {
        Enemy* enemy = nullptr;
        int c_pos = c(engine);
        int h_pos = h(engine);

        int j = 0;

        for (auto it = colors.begin(); it != colors.end(); it++) {
            if (j == c_pos) {
                enemy = new Enemy(it->first, it->second);
                break;
            }

            j++;
        }

        enemy->Init();
        enemy->SetScale({ 80, 80 });
        enemy->SetPosition({ 1280 + (700 * i), heights[h_pos] });
        enemies.push_back(enemy);
        objects.push_front(enemy);
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

void Hw1::UpdateEnemies(float deltaTimeSeconds)
{
    for (auto enemy : enemies) {
        if (!enemy->enabled)
            continue;

        switch (enemy->state) {
        case ENABLED:
            enemy->AddPosition({ -200 * deltaTimeSeconds, 0 });
            enemy->AddRotation(90 * deltaTimeSeconds * TO_RADIANS);
            break;
        case HIT:
            enemy->AddScale({ -300 * deltaTimeSeconds, -300 * deltaTimeSeconds });
            break;
        case DISABLED:
            break;
        }
    }
}

void Hw1::UpdateProjectiles(float deltaTimeSeconds)
{
    for (auto projectile : projectiles) {
        switch (projectile->state) {
        case ENABLED:
            projectile->AddPosition({ 400 * deltaTimeSeconds, 0 });
            projectile->AddRotation(2 * -deltaTimeSeconds);
            break;
        case HIT:
            projectile->AddScale({ -500 * deltaTimeSeconds, -500 * deltaTimeSeconds });
        case DISABLED:
            break;
        }
    }
}

void Hw1::UpdateSpots(float deltaTimeSeconds)
{
    {
        for (auto spot : spots) {
            Launcher* l = spot->GetLauncher();

            if (l != nullptr && l->state == ENABLED) {
                l->Update(deltaTimeSeconds);

                glm::vec3 color = l->GetColor();

                for (auto enemy : enemies) {
                    if (enemy->GetColor() == color &&
                        enemy->GetPosition().y == spot->GetPosition().y) {
                        Projectile* projectile = l->Launch();

                        if (projectile != nullptr) {
                            objects.push_front(projectile);
                            projectiles.push_back(projectile);
                        }

                        break;
                    }
                }
            }
        }
    }

    {
        for (auto spot : spots) {
            Launcher* l = spot->GetLauncher();

            if (!l)
                continue;

            switch (l->state) {
            case ENABLED:
                break;
            case HIT:
                l->AddScale({ -5 * deltaTimeSeconds, -5 * deltaTimeSeconds });
                break;
            case DISABLED:
                for (auto it_obj = objects.begin(); it_obj != objects.end(); it_obj++) {
                    Object2D* obj = *it_obj;

                    if (obj == l) {
                        objects.erase(it_obj);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void Hw1::UpdateObjects(float deltaTimeSeconds)
{
    for (auto* object : objects) {
        if (!object->enabled)
            continue;

        for (auto [mesh, modelMatrix] : object->GetRenderDetails())
            RenderMesh2D(mesh, shaders["VertexColor"], modelMatrix);
    }
}
