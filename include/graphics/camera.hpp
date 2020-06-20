#pragma once

#include "system/input.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    void Update(Input& input, float deltaTime, float width, float height);
    void Update(float width, float height);

    const glm::mat4& GetProjection() const;
    const glm::mat4& GetView() const;
    const glm::mat4& GetOrthographic() const;
    void SetPosition(float, float, float);
    void SetPosition(glm::vec3);

    void SetYaw(float);
    void SetPitch(float);

    float* GetFOV();

private:
    glm::vec3 m_position = glm::vec3( 0, 0, 5 ); 
    float m_horizontalAngle = 3.14f;
    float m_verticalAngle = 0.0f;
    float m_fov = 60.0f;
    float m_speed = 3.0f; // 3 units / second
    float m_mouseSpeed = 0.01f;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_orthoMatrix;
    glm::mat4 m_projMatrix;
};