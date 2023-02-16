#pragma once

#include <glm/glm.hpp>

namespace Hazel {
  class OrthographicCamera
  {
  public:
    OrthographicCamera(float left, float right, float bottom, float top);


    const glm::vec3& GetPostition() const { return m_Position; }
    void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

    float GetRotation() const {
      return m_Rotation;
    }
    void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectMatrix() const { return m_ViewProjectionMatrix; }

  private:
    void RecalculateViewMatrix();
  private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = { .0f,.0f,.0f };
    float m_Rotation = .0f;
  };
}

