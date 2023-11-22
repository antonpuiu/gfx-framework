#include "hw_m1/hw2/3d/transform3d.h"

#include <iostream>
#include <stdexcept>

using namespace m1;

Transform3D::Transform3D()
    : Transform3D(nullptr)
{
}

Transform3D::Transform3D(Transform3D* parent)
    : parent(parent)
    , modelMatrix(glm::mat3(1))
    , scale({ 1.f, 1.f, 1.f })
    , pos({ 0, 0, 0 })
    , rotation({ 0, 0, 0 })
    , dirtyFlag(true)
    , detatched(false)
{
}

Transform3D::Transform3D(Transform3D& transform2D)
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

glm::mat4 Transform3D::GetModelMatrix()
{
    if (dirtyFlag)
        ComputeModelMatrix();

    return modelMatrix;
}

void Transform3D::ComputeModelMatrix()
{
    glm::mat4 rot;
    glm::mat4 trs;
    glm::mat4 scl;

    if (detatched) {
        rot = glm::rotate(glm::mat4(1), glm::radians(rotationDetatched.x), glm::vec3(1, 0, 0)) *
              glm::rotate(glm::mat4(1), glm::radians(rotationDetatched.y), glm::vec3(0, 1, 0)) *
              glm::rotate(glm::mat4(1), glm::radians(rotationDetatched.z), glm::vec3(0, 0, 1));
        trs = glm::translate(glm::mat4(1), posDetatched);
        scl = glm::scale(glm::mat4(1), scaleDetatched);
    } else {
        rot = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
              glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
              glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));
        trs = glm::translate(glm::mat4(1), pos);
        scl = glm::scale(glm::mat4(1), scale);
    }

    if (parent != nullptr && !detatched) {
        modelMatrix = parent->modelMatrix * trs * rot * scl;
    } else {
        modelMatrix = trs * rot * scl;
    }

    for (Transform3D* children : childrens)
        children->ComputeModelMatrix();

    dirtyFlag = false;
}

void Transform3D::SetPosition(glm::vec3 pos)
{
    if (detatched)
        posDetatched = pos;
    else
        this->pos = pos;

    dirtyFlag = true;
}

void Transform3D::AddPosition(glm::vec3 qty)
{
    if (detatched)
        posDetatched += qty;
    else
        pos += qty;

    dirtyFlag = true;
}

glm::vec3 Transform3D::GetPosition()
{
    if (detatched)
        return posDetatched;

    return pos;
}

void Transform3D::SetScale(glm::vec3 scale)
{
    if (detatched)
        scaleDetatched = scale;
    else
        this->scale = scale;

    dirtyFlag = true;
}

void Transform3D::AddScale(glm::vec3 qty)
{
    if (detatched)
        scaleDetatched += qty;
    else
        scale += qty;

    dirtyFlag = true;
}

glm::vec3 Transform3D::GetScale()
{
    if (detatched)
        return scaleDetatched;

    return scale;
}

void Transform3D::SetRotation(glm::vec3 rotation)
{
    if (detatched)
        rotationDetatched = rotation;
    else
        this->rotation = rotation;

    dirtyFlag = true;
}

void Transform3D::AddRotation(glm::vec3 qty)
{
    if (detatched)
        rotationDetatched += qty;
    else
        rotation += qty;

    dirtyFlag = true;
}

glm::vec3 Transform3D::GetRotation()
{
    if (detatched)
        return rotationDetatched;

    return rotation;
}

void Transform3D::AddChildren(Transform3D* children)
{
    if (children == nullptr)
        throw std::runtime_error("Cannot insert null children.");

    children->parent = this;

    bool insert = true;

    for (auto child : childrens) {
        if (children == child) {
            insert = false;
            break;
        }
    }

    if (insert)
        childrens.push_back(children);
}

void Transform3D::AttatchToParent()
{
    detatched = false;
    dirtyFlag = true;
}

void Transform3D::AttatchToParent(Transform3D* parent)
{
    this->parent = parent;
    detatched = false;
    dirtyFlag = true;
}

void Transform3D::DetatchFromParent()
{
    scaleDetatched = parent->GetScale() + scale;
    posDetatched = parent->GetPosition() + pos;
    rotationDetatched = parent->GetRotation() + rotationDetatched;

    detatched = true;
    dirtyFlag = true;
}
