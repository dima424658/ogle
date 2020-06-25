#pragma once

#include <string>
#include <random>

#include <script/script.hpp>
#include <sound/sound3d.hpp>
#include <sound/sound2d.hpp>
#include <graphics/mesh.hpp>
#include <graphics/texture.hpp>

#include <system/timer.hpp>

#undef LoadImage

class CObject
{
public:
	CObject(const std::string& name, bool active = true);
	~CObject();
	uint32_t GetID() const;

	bool IsActive() const;
	void SetActive(bool active);
	const std::string& GetName() const;
	void SetName(const std::string& name);

	void SetPosition(glm::vec3& pos);
	const glm::vec3& GetPosition() const;
	glm::vec3& GetPosition();

	void CreateSound3D(Sound::CSound3D* c);
	void DestroySound3D();
	Sound::CSound3D* GetSound3D() const;

	void CreateMesh(Graphics::CMesh* c);
	void DestroyMesh();
	Graphics::CMesh* GetMesh() const;

	void CreateTexture(Graphics::CTexture* c);
	void DestroyTexture();
	Graphics::CTexture* GetTexture() const;

	void Update();

private:
	uint32_t m_id;
	glm::vec3 m_position;
	std::string m_name;
	bool m_active;
	
	Sound::CSound2D* m_sound2d = nullptr;
	Sound::CSound3D* m_sound3d = nullptr;
	Graphics::CTexture* m_texture = nullptr;
	Graphics::CMesh* m_mesh = nullptr;

};