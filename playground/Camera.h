#pragma once
#include "stdafx.h"
#include <glm/glm.hpp>

class CCamera
{
public:
    CCamera(float fFieldOfView, float fNearClippingplane, float fFarClippingPlane, float fAspectRatio, glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector);
    CCamera(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar, glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector);
    ~CCamera();

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

    void Rotate(glm::vec3 RotationAxe, float fAngle);
    void Translate(glm::vec3 Translation);
    void AddPitch(float fPitch);
    void SetInfrontOfModel(glm::vec3 maxValues, glm::vec3 minValues);
    float GetVerticalFieldOfView() const;
    bool IsPerspective();
    void SetOrthographic(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar);
    void SetPerspective(float fFieldOfView, float fAspectRatio, float fNearClippingplane, float fFarClippingPlane);
    void SetOrientation(glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector);

private:
    const float m_fPI = 3.1415926f;
    float m_fPitch = 0.0f;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    float m_fHorizontalFieldOfView = 0.0f;
    float m_fAspectRatio = 0.0f;
    bool m_bPerspective = true;
};
