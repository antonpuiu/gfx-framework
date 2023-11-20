#include "hw_m1/hw1/2d/transform2d/transform2d.h"
#include "lab_m1/lab3/transform2D.h"

#include <iostream>
#include <stdexcept>

using namespace m1;

Transform2D::Transform2D()
    : Transform2D(nullptr)
{
}

Transform2D::Transform2D(Transform2D* parent)
    : parent(parent)
    , modelMatrix(glm::mat3(1))
    , scale({ 1.f, 1.f })
    , pos({ 0, 0 })
    , rotation(0.0)
    , dirtyFlag(true)
    , detatched(false)
{
}

Transform2D::Transform2D(Transform2D& transform2D)
    : parent(transform2D.parent)
    , modelMatrix(transform2D.modelMatrix)
    , scale(transform2D.scale)
    , pos(transform2D.pos)
    , rotation(transform2D.rotation)
    , scaleDetatched(transform2D.scaleDetatched)
    , posDetatched(transform2D.posDetatched)
    , rotationDetatched(transform2D.rotationDetatched)
    , dirtyFlag(transform2D.dirtyFlag)
    , detatched(transform2D.detatched)
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
    glm::mat3 rot;
    glm::mat3 trs;
    glm::mat3 scl;

    if (detatched) {
        rot = transform2D::Rotate(rotationDetatched);
        trs = transform2D::Translate(posDetatched.x, posDetatched.y);
        scl = transform2D::Scale(scaleDetatched.x, scaleDetatched.y);
    } else {
        rot = transform2D::Rotate(rotation);
        trs = transform2D::Translate(pos.x, pos.y);
        scl = transform2D::Scale(scale.x, scale.y);
    }

    if (parent != nullptr && !detatched) {
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
    if (detatched)
        posDetatched = pos;
    else
        this->pos = pos;

    dirtyFlag = true;
}

void Transform2D::AddPosition(glm::vec2 qty)
{
    if (detatched)
        posDetatched += qty;
    else
        pos += qty;

    dirtyFlag = true;
}

glm::vec2 Transform2D::GetPosition()
{
    if (detatched)
        return posDetatched;

    return pos;
}

void Transform2D::SetScale(glm::vec2 scale)
{
    if (detatched)
        scaleDetatched = scale;
    else
        this->scale = scale;

    dirtyFlag = true;
}

void Transform2D::AddScale(glm::vec2 qty)
{
    if (detatched)
        scaleDetatched += qty;
    else
        scale += qty;

    dirtyFlag = true;
}

glm::vec2 Transform2D::GetScale()
{
    if (detatched)
        return scaleDetatched;

    return scale;
}

void Transform2D::SetRotation(float rotation)
{
    if (detatched)
        rotationDetatched = rotation;
    else
        this->rotation = rotation;

    dirtyFlag = true;
}

void Transform2D::AddRotation(float qty)
{
    if (detatched)
        rotationDetatched += qty;
    else
        rotation += qty;

    dirtyFlag = true;
}

float Transform2D::GetRotation()
{
    if (detatched)
        return rotationDetatched;

    return rotation;
}

void Transform2D::AddChildren(Transform2D* children)
{
    if (children == nullptr)
        throw std::runtime_error("Cannot insert null children.");

    children->parent = this;
    childrens.push_back(children);
}

void Transform2D::AttatchToParent()
{
    detatched = false;
    dirtyFlag = true;
}

void Transform2D::AttatchToParent(Transform2D* parent)
{
    this->parent = parent;
    detatched = false;
    dirtyFlag = true;
}

void Transform2D::DetatchFromParent()
{
    scaleDetatched = parent->GetScale() + scale;
    posDetatched = parent->GetPosition() + pos;
    rotationDetatched = parent->GetRotation() + rotationDetatched;

    detatched = true;
    dirtyFlag = true;
}
