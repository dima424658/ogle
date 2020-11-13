#include <scene/camera.hpp>

CCamera::CCamera()
{
    m_size = { 800.f, 600.f };
    m_yVector = { 0.0f, -1.0f, 0.0f };
    m_lookAt = { 0.0f, 0.0f, 0.0f };
    m_position = { 10.0f, 10.0f, 10.0f };

    m_fov = glm::radians(45.0f);
    m_sensivity = 0.5f;

    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));
    m_near = 0.1f;
    m_far = 100.0f;

    Update();
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

    Update();
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

    Update();
}

void CCamera::UpdateZoom(float delta)
{
    m_direction = (m_position - m_lookAt);;
    m_right = glm::normalize(glm::cross(m_yVector, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    m_position = glm::normalize(m_direction) * (std::max(glm::length(m_direction) + delta * m_sensivity, 1.0f)) + m_lookAt;

    Update();
}

void CCamera::Update()
{
    float aspect = m_size.x / m_size.y;

    m_projection = glm::perspective(m_fov, aspect, m_near, m_far);
    m_orthographic = glm::ortho(-1.0f, 1.0f * aspect, -1.0f * aspect, 1.0f, m_near, m_far * 10);
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

const glm::vec3& CCamera::GetUp() const
{
    return m_up;
}

const glm::vec3& CCamera::GetRight() const
{
    return m_right;
}

const glm::mat4& CCamera::GetProjection() const
{
    return m_projection;
}

const glm::mat4& CCamera::GetView() const
{
    return m_view;
}

const glm::mat4& CCamera::GetOrthographic() const
{
    return m_orthographic;
}

const glm::vec3& CCamera::GetPosition() const
{
    return m_position;
}

glm::vec3 CCamera::GetPickRay(const glm::vec2& mousePos) const
{
    glm::vec3 result;
    result.x = ( ( ( 2.0f * mousePos.x ) / m_size.x ) - 1 );
    result.y = -( ( ( 2.0f * mousePos.y ) / m_size.y ) - 1 );
    result.z = 1.0f;

    // Back project the ray from screen to the far clip plane
    result.x /= m_projection[0][0];
    result.y /= m_projection[1][1];

    glm::mat4 inv = glm::inverse(m_view);

    result *= m_far;
    result = glm::vec4(result, 1.0f) * inv;

    return result;
}