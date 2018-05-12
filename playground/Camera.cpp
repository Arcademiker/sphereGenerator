//#include "stdafx.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>
#include <math.h>
//#include "Basics.h"
#include <algorithm>

CCamera::CCamera(float fFieldOfView, float fAspectRatio, float fNearClippingplane, float fFarClippingPlane, glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector)
{
    this->SetOrientation(EyePosition, LookAtVector, UpVector);
    this->SetPerspective(fFieldOfView, fAspectRatio, fNearClippingplane, fFarClippingPlane);
}

CCamera::CCamera(float fLeft, float fRight,float fBottom, float fTop, float fNear, float fFar, glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector)
{
    this->SetOrientation(EyePosition, LookAtVector, UpVector);
    this->SetOrthographic(fLeft, fRight, fBottom, fTop, fNear, fFar);
}

void CCamera::SetPerspective(float fFieldOfView, float fAspectRatio, float fNearClippingplane, float fFarClippingPlane)
{
    m_ProjectionMatrix = glm::perspective(fFieldOfView /** m_fPI / 180.0f*/, fAspectRatio, fNearClippingplane, fFarClippingPlane);
    m_fHorizontalFieldOfView = fFieldOfView /** m_fPI / 180.0f*/;
    m_fAspectRatio = fAspectRatio;
    m_bPerspective = true;
}

void CCamera::SetOrthographic(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar)
{
    m_ProjectionMatrix = glm::ortho(fLeft, fRight, fBottom, fTop, fNear, fFar);
    m_fHorizontalFieldOfView = 0;
    m_fAspectRatio = (fRight - fLeft) / (fTop - fBottom);
    m_bPerspective = false;
}

void CCamera::SetOrientation(glm::vec3 EyePosition, glm::vec3 LookAtVector, glm::vec3 UpVector)
{
    m_ViewMatrix = glm::lookAt(EyePosition, LookAtVector, UpVector);
}

bool CCamera::IsPerspective()
{
    return m_bPerspective;
}

CCamera::~CCamera()
{
}

glm::mat4 CCamera::GetViewMatrix() const
{
    return glm::rotate(m_fPitch, glm::vec3(1.0f, 0.0f, 0.0f)) * m_ViewMatrix;
}

glm::mat4 CCamera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

glm::mat4 CCamera::GetViewProjectionMatrix() const
{
    return m_ProjectionMatrix * glm::rotate(m_fPitch, glm::vec3(1.0f, 0.0f, 0.0f)) * m_ViewMatrix;
}

void CCamera::Rotate(glm::vec3 RotationAxe, float fAngle)
{
    m_ViewMatrix = glm::rotate(fAngle /** m_fPI / 180.0f*/, RotationAxe) * m_ViewMatrix;
}

void CCamera::Translate(glm::vec3 Translation)
{
    m_ViewMatrix = glm::translate(Translation)* m_ViewMatrix;
}

void CCamera::AddPitch(float fPitch)
{
    m_fPitch += fPitch   *m_fPI / 180.0f ;
    m_fPitch = m_fPitch > m_fPI / 2.0f ? m_fPI / 2.0f : m_fPitch;
    m_fPitch = m_fPitch < -m_fPI / 2.0f ? -m_fPI / 2.0f : m_fPitch;
}

float CCamera::GetVerticalFieldOfView() const
{
    return m_fHorizontalFieldOfView;
}

void CCamera::SetInfrontOfModel(glm::vec3 maxValues, glm::vec3 minValues)
{
    glm::vec3 center = (maxValues + minValues) / 2.0f;
    glm::vec3 scale = maxValues - minValues;
    glm::vec3 EyePosition = glm::vec3(center.x, center.y- scale.y, center.z);
    glm::vec3 LookAtVector = glm::vec3(0, center.y,0);

    glm::vec3 UpVector = glm::vec3(0, 1, 0);
    m_ViewMatrix = glm::lookAt(EyePosition, LookAtVector, UpVector);

    float maxScaleDimension = std::max(scale.x, scale.y);

    m_ProjectionMatrix = glm::perspective(this->m_fHorizontalFieldOfView, this->m_fAspectRatio, 0.01f, maxScaleDimension*20);

}