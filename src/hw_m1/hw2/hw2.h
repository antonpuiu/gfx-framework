#pragma once

#include "components/simple_scene.h"
#include "components/camera_input.h"
#include "lab_m1/lab5/lab_camera.h"
#include "hw_m1/hw2/3d/object3d.h"

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
        gfxc::ImplementedCameraInput* cameraInput;

        std::vector<Object3D*> objects;
    };
} // namespace m1
