#include <scene/camera.hpp>

CCamera::CCamera()
{
    m_size = { 800.f, 600.f };
    m_yVector = { 0.0f, 1.0f, 0.0f };
    m_lookAt = { 0.0f, 0.0f, 0.0f };
    m_position = { 10.0f, 10.0f, 10.0f };

    m_fov = glm::radians(45.0f);
    m_sensivity = 0.5f;

    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));


    m_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
    m_view = glm::lookAt(m_position, m_lookAt, m_yVector);
}

CCamera::~CCamera()
{

}

void CCamera::UpdatePan(const glm::vec2& delta)
{
    float tanFovX = m_size.x / m_size.y * glm::tan(m_fov / 2); // tan of x fov

    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    float x = (delta.x / m_size.x) * (2 * glm::length(m_direction) * tanFovX);
    float y = (delta.y / m_size.y) * (2 * glm::length(m_direction) * glm::tan(m_fov / 2));

    m_lookAt -= m_up * y + m_right * x;
    m_position -= m_up * y + m_right * x;

    m_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
    m_view = glm::lookAt(m_position, m_lookAt, m_yVector);
}

void CCamera::UpdateRotate(const glm::vec2& delta)
{
    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), glm::radians(delta.x) * m_sensivity, m_up);
    matrix = glm::rotate(matrix, glm::radians(delta.y) * m_sensivity, m_right);
    m_direction = glm::vec4(m_direction, 0.0f) * matrix;

    m_position = m_direction + m_lookAt;

    m_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
    m_view = glm::lookAt(m_position, m_lookAt, m_yVector);
}

void CCamera::UpdateZoom(const glm::vec2& delta)
{
    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    m_position = glm::normalize(m_direction) * (std::max(glm::length(m_direction) + delta.y * m_sensivity, 1.0f)) + m_lookAt;

    m_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
    m_view = glm::lookAt(m_position, m_lookAt, m_yVector);
}

void CCamera::Update()
{
    m_projection = glm::perspective(glm::radians(45.0f), m_size.x / m_size.y, 0.1f, 100.0f);
    m_view = glm::lookAt(m_position, m_lookAt, m_yVector);
}

void CCamera::SetRenderSize(float x, float y)
{
    m_size.x = x;
    m_size.y = y;
    //Update();
}

void CCamera::SetFOV(float degrees)
{
    m_fov = glm::radians(degrees);
    //Update();
}

void CCamera::SetSensivity(float val)
{
    m_sensivity = val;
    //Update();
}

const glm::mat4& CCamera::GetProjection()
{
    return m_projection;
}

const glm::mat4& CCamera::GetView()
{
    return m_view;
}

const glm::vec3& CCamera::GetPosition()
{
    return m_position;
}
