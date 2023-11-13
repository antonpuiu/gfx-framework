#include "star.h"

#include "core/gpu/vertex_format.h"
#include "components/simple_scene.h"

using namespace gfxc;

Star::Star()
    : Star(VertexColor::WHITE)
{
}

Star::Star(glm::vec3 color)
    : color(color)
{
}

Star::~Star()
{
}

void Star::Init()
{
    std::string name =
        "star" + std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

    if (SimpleScene::meshes.find(name) != SimpleScene::meshes.end()) {
        mesh = SimpleScene::meshes.at(name);
        return;
    }

    mesh = new Mesh(name);

    std::vector<VertexFormat> vertices;

    int sides = 5;
    float radius = .25f;

    for (int i = 0; i < sides; i++) {
        float angle = M_PI * 2.0f / sides * i - M_PI / 2;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        vertices.push_back({ { x, y, 0 }, color });
    }

    std::vector<unsigned int> indices{
        0, 1, 2, // first triangle
        2, 3, 4, // second triangle
        4, 0, 2 // third triangle
    };

    for (int i = 0; i < sides; i++) {
        int A = i;
        int B = (i + 1) % sides;

        float slope = (vertices[A].position.y - vertices[B].position.y) /
                      (vertices[A].position.x - vertices[B].position.x);

        float x, y;

        x = std::min(vertices[A].position.x, vertices[B].position.x) +
            (abs(vertices[A].position.x - vertices[B].position.x) / 2);
        y = std::min(vertices[A].position.y, vertices[B].position.y) +
            (abs(vertices[A].position.y - vertices[B].position.y) / 2);

        float x_p = 0.0, y_p;

        float a, b, c;
        float height = .25f;

        a = 1.f + 1.f / (slope * slope);
        b = (-2.f * x - (2.f / slope) * (x / slope + y) + 2.f * y / slope);
        c = (pow(x / slope + y, 2.f) - 2.f * y * (x / slope + y) - pow(height, 2) + pow(x, 2) +
             pow(y, 2));

        if (i < 2)
            x_p = (-b + sqrt(pow(b, 2.f) - 4 * a * c)) / (2.f * a);
        else if (i > 2)
            x_p = (-b - sqrt(pow(b, 2.f) - 4 * a * c)) / (2.f * a);

        y_p = x / slope + y - (1.f / slope) * x_p;

        if (i == 2) {
            x_p = x;
            y_p = y + height;
        }

        vertices.push_back({ { x_p, y_p, 0 }, color });

        indices.push_back(A);
        indices.push_back(B);
        indices.push_back(vertices.size() - 1);
    }

    mesh->InitFromData(vertices, indices);

    SimpleScene::meshes[name] = mesh;
}
