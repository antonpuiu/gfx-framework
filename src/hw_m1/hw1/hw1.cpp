#include "hw_m1/hw1/hw1.h"

#include "core/gpu/vertex_format.h"
#include "hw_m1/hw1/object2D.h"
#include "hw_m1/hw1/transform2D.h"
#include "utils/math_utils.h"
#include <cmath>
#include <vector>
#include <iostream>

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
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    meshes["square"] =
        object2D::CreateObject(object2D::ObjectType::SQUARE, "square", VertexColor::RED);

    meshes["star"] = object2D::CreateObject(object2D::ObjectType::STAR, "star", VertexColor::CYAN);
    meshes["hexa"] =
        object2D::CreateObject(object2D::ObjectType::HEXAGON, "hexa", VertexColor::PURPLE);
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
    glm::mat3 modelMatrix;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(150, 250);
    modelMatrix *= transform2D::Scale(100, 100);

    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(300, 250);
    modelMatrix *= transform2D::Scale(100, 100);

    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500, 250);
    modelMatrix *= transform2D::Scale(100, 100);

    RenderMesh2D(meshes["hexa"], shaders["VertexColor"], modelMatrix);
}

void Hw1::FrameEnd()
{
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
    // Add mouse move event
}

void Hw1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Hw1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Hw1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Hw1::OnWindowResize(int width, int height)
{
}
