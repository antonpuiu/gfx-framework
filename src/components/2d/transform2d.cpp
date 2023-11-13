#include "components/2d/transform2d.h"
#include "lab_m1/lab3/transform2D.h"

#include <stdexcept>

using namespace gfxc;

Transform2D::Transform2D()
    : Transform2D(nullptr)
{
}

Transform2D::Transform2D(Transform2D* parent)
    : parent(parent)
{
}

glm::mat3 Transform2D::GetModelMatrix()
{
    if (dirtyFlag)
        ComputeModelMatrix();

    return modelMatrix;
}

void Transform2D::ComputeModelMatrix()
{
    const glm::mat3 rot = transform2D::Rotate(rotation);
    const glm::mat3 trs = transform2D::Translate(pos.x, pos.y);
    const glm::mat3 scl = transform2D::Scale(scale.x, scale.y);

    if (parent) {
        modelMatrix = parent->modelMatrix * trs * rot * scl;
    } else {
        modelMatrix = trs * rot * scl;
    }

    for (Transform2D* children : childrens)
        children->ComputeModelMatrix();

    dirtyFlag = false;
}

void Transform2D::SetPosition(glm::vec2 pos)
{
    this->pos = pos;
    dirtyFlag = true;
}

void Transform2D::AddPosition(glm::vec2 qty)
{
    pos += qty;
    dirtyFlag = true;
}

glm::vec2 Transform2D::GetPosition()
{
    return pos;
}

void Transform2D::SetScale(glm::vec2 scale)
{
    this->scale = scale;
    dirtyFlag = true;
}

void Transform2D::AddScale(glm::vec2 qty)
{
    scale += qty;
    dirtyFlag = true;
}

glm::vec2 Transform2D::GetScale()
{
    return scale;
}

void Transform2D::SetRotation(float rotation)
{
    this->rotation = rotation;
    dirtyFlag = true;
}

void Transform2D::AddRotation(float qty)
{
    rotation += qty;
    dirtyFlag = true;
}

float Transform2D::GetRotation()
{
    return rotation;
}

void Transform2D::AddChildren(Transform2D* children)
{
    if (children == nullptr)
        throw std::runtime_error("Cannot insert null children.");

    children->parent = this;
    childrens.push_back(children);
}
