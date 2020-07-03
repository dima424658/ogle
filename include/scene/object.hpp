#pragma once

#include <string>
#include <random>

#include <script/script.hpp>

#include <sound/sound3d.hpp>
#include <sound/sound2d.hpp>

#include <graphics/light.hpp>
#include <graphics/mesh.hpp>
#include <graphics/material.hpp>

#include <scene/camera.hpp>

#include <system/timer.hpp>

#include <glm/gtc/type_ptr.hpp>

#undef LoadImage

class CObject
{
public:
	// TODO спрятать установку кастомного id в private секцию.
	CObject(const std::string& name, bool active = true, uint32_t id = 0);
	~CObject();
	uint32_t GetID() const;

	bool IsActive() const;
	void SetActive(bool active);
	const std::string& GetName() const;
	void SetName(const std::string& name);

	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetPosition() const;
	glm::vec3& GetPosition();
	
	void SetRotation(const glm::vec3& rot);
	const glm::vec3& GetRotation() const;
	glm::vec3& GetRotation();

	void SetScale(const glm::vec3& scale);
	const glm::vec3& GetScale() const;
	glm::vec3& GetScale();

	void CreateSound3D(Sound::CSound3D* c);
	void DestroySound3D();
	Sound::CSound3D* GetSound3D() const;

	void CreateMesh(Graphics::CMesh* c);
	void DestroyMesh();
	Graphics::CMesh* GetMesh() const;

	void CreateMaterial(Graphics::CMaterial* c);
	void DestroyMaterial();
	Graphics::CMaterial* GetMaterial() const;
	
	void CreateLight(Graphics::CLight* c);
	void DestroyLight();
	Graphics::CLight* GetLight() const;
/*
	void CreateCamera(CCamera* c);
	void DestroyCamera();
	CCamera* GetCamera() const;
*/
	void Update();

private:
	uint32_t m_id;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	std::string m_name;
	bool m_active;
	
	Sound::CSound2D* m_sound2d = nullptr;
	Sound::CSound3D* m_sound3d = nullptr;
	Graphics::CMaterial* m_material = nullptr;
	Graphics::CMesh* m_mesh = nullptr;
	Graphics::CLight* m_light = nullptr;
	//CCamera* m_camera = nullptr;

};