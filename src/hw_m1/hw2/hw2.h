#pragma once

#include "components/simple_scene.h"
#include "components/camera_input.h"
#include "lab_m1/lab5/lab_camera.h"
#include "hw_m1/hw2/3d/object3d.h"

#include <random>

namespace m1
{
    class Hw2 : public gfxc::SimpleScene {
    public:
        Hw2();
        ~Hw2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
                              glm::vec3 color = VertexColor::WHITE);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    private:
        implemented::Camera* camera;

        std::list<Object3D*> objects;
        std::list<Tank*> enemies;
        std::list<TankProjectile*> projectiles;
        std::vector<Building*> buildings;

        Tank* tank;
        Walls* walls;

        static inline std::random_device rd = std::random_device();
        static inline std::default_random_engine engine = std::default_random_engine(Hw2::rd());
        static inline std::uniform_real_distribution<float> count =
            std::uniform_real_distribution<float>(1, 10);

        float timeLeft;
    };
} // namespace m1
