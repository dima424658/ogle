#include "graphics/camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
    
}

void Camera::Update(Input& input, float deltaTime, float width, float height)
{
	m_horizontalAngle += m_mouseSpeed * input.GetDeltaX();
	m_verticalAngle   += m_mouseSpeed * input.GetDeltaY();

	if(m_verticalAngle < -M_PI_2)
		m_verticalAngle = -M_PI_2;

	if(m_verticalAngle > M_PI_2)
		m_verticalAngle = M_PI_2;
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(m_verticalAngle) * sin(m_horizontalAngle), 
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f/2.0f), 
		0,
		cos(m_horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (input.GetKeyDown(GLFW_KEY_W))
		m_position += direction * deltaTime * m_speed;
	// Move backward
	if (input.GetKeyDown(GLFW_KEY_S))
		m_position -= direction * deltaTime * m_speed;
	// Strafe right
	if (input.GetKeyDown(GLFW_KEY_D))
		m_position += right * deltaTime * m_speed;
	// Strafe left
	if (input.GetKeyDown(GLFW_KEY_A))
		m_position -= right * deltaTime * m_speed;


	m_projMatrix = glm::perspective(glm::radians(m_fov), width / height, 0.1f, 100.0f);
	m_orthoMatrix = glm::ortho(0, static_cast<int>(width), static_cast<int>(height), 0);

	// Camera matrix
	m_viewMatrix       = glm::lookAt(
								m_position,
								m_position + direction,
								up
						   );
}

void Camera::Update(float width, float height)
{
	m_projMatrix = glm::perspective(glm::radians(m_fov), width / height, 0.1f, 100.0f);
	m_orthoMatrix = glm::ortho(0, static_cast<int>(width), static_cast<int>(height), 0);
}

const glm::mat4& Camera::GetProjection() const
{
    return m_projMatrix;
}

const glm::mat4& Camera::GetView() const
{
    return m_viewMatrix;
}

const glm::mat4& Camera::GetOrthographic() const
{
    return m_orthoMatrix;
}

float* Camera::GetFOV()
{
	return &m_fov;
}

