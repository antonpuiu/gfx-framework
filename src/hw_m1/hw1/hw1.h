#pragma once

#include "components/simple_scene.h"
#include "hw_m1/hw1/2d/render_object2d/render_object2d.h"
#include "hw_m1/hw1/2d/render_object2d/render_objects2d.h"
#include "hw_m1/hw1/2d/object2d.h"

#include <unordered_map>

namespace m1
{
    class Hw1 : public gfxc::SimpleScene {
    public:
        Hw1();
        ~Hw1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void EnableNewEnergy(int count);
        void EnableEnemies(int count);

        int GetCurrentEnergyLevel();

    private:
        // General objects, used for rendering purposes.
        std::list<Object2D*> objects;

        std::vector<Object2D*> lifes;

        std::vector<Object2D*> currentEnergy;
        std::vector<Object2D*> newEnergy;

        std::vector<Object2D*> enemies;

        Limit* limit;
        Launcher* launcher;
        std::vector<Spot*> spots;

        std::vector<Generator*> generators;
    };
} // namespace m1
