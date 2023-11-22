#include "hw2.h"

#include "lab_m1/lab1/lab1.h"

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
        camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        fov = 60.f;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
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
}

void Hw2::FrameStart()
{
    {
        glm::ivec2 resolution = window->props.resolution;
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, resolution.x, resolution.y);
    }
}

void Hw2::Update(float deltaTimeSeconds)
{
}

void Hw2::FrameEnd()
{
    DrawCoordinateSystem();
}

void Hw2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
                       glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
                       glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Hw2::OnInputUpdate(float deltaTime, int mods)
{
}

void Hw2::OnKeyPress(int key, int mods)
{
}

void Hw2::OnKeyRelease(int key, int mods)
{
}

void Hw2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Hw2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Hw2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Hw2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Hw2::OnWindowResize(int width, int height)
{
}
