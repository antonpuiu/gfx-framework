#include "hw2.h"

#include "core/gpu/mesh.h"
#include "hw_m1/hw2/3d/object3d.h"
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
        cameraInput = new gfxc::ImplementedCameraInput(camera);
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
        Plane* plane = new Plane();

        plane->SetScale({ 10, 10, 10 });
        plane->SetPosition({ 0, 0, 0 });

        objects.push_back(plane);
    }

    {
        tank = new Tank();
        objects.push_back(tank);
    }

    {
        for (auto object : objects)
            object->Init();
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

    // {
    //     glLineWidth(3);
    //     glPointSize(5);
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // }
}

void Hw2::Update(float deltaTimeSeconds)
{
}

void Hw2::FrameEnd()
{
    for (auto object : objects)
        for (auto primitive : object->GetPrimitives())
            RenderSimpleMesh(primitive->GetMesh(), shaders["HwShader"], primitive->GetModelMatrix(),
                             primitive->GetColor());
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
