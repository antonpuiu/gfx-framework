#pragma once

#include "utils/glm_utils.h"

struct VertexFormat {
    VertexFormat(glm::vec3 position, glm::vec3 color = glm::vec3(1),
                 glm::vec3 normal = glm::vec3(0, 1, 0), glm::vec2 text_coord = glm::vec2(0))
        : position(position)
        , normal(normal)
        , text_coord(text_coord)
        , color(color)
    {
    }

    // Position of the vertex
    glm::vec3 position;

    // Vertex normal
    glm::vec3 normal;

    // Vertex texture coordinates
    glm::vec2 text_coord;

    // Vertex color
    glm::vec3 color;
};

namespace VertexColor
{
    extern glm::vec3 BLACK;
    extern glm::vec3 GRAY;
    extern glm::vec3 RED;
    extern glm::vec3 GREEN;
    extern glm::vec3 BLUE;
    extern glm::vec3 WHITE;
    extern glm::vec3 PURPLE;
    extern glm::vec3 YELLOW;
    extern glm::vec3 ORANGE;
    extern glm::vec3 CYAN;
};
