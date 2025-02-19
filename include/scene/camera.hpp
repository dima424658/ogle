#pragma once

#include <glad/glad.h>

#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void UpdatePan(const glm::vec2& mouseDelta);
	void UpdateRotate(const glm::vec2& mouseDelta);
	void UpdateZoom(float delta);
	void Update();

	void SetRenderSize(float, float);
	void SetFOV(float degrees);
	void SetSensivity(float);

	const glm::vec3& GetUp() const;
	const glm::vec3& GetRight() const;
	const glm::mat4& GetProjection() const;
	const glm::mat4& GetView() const;
	const glm::mat4& GetOrthographic() const;
	const glm::vec3& GetPosition() const;

	glm::vec3 GetPickRay(const glm::vec2& mousePos) const;

private:
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_orthographic;

	glm::vec3 m_position;
	glm::vec3 m_lookAt;
	glm::vec3 m_yVector;

	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;

	float m_far;
	float m_near;
	float m_fov;
	float m_sensivity;

	glm::vec2 m_size;

	GLuint m_texture;
};