#include <scene/object.hpp>

CObject::CObject(const std::string& name, bool active) : m_name(name), m_active(active), m_position(0.0f)
{
	static std::random_device rd;
	static std::mt19937_64 generator = std::mt19937_64(rd());
	static std::uniform_int_distribution<uint32_t> uid;

	m_id = uid(generator);

}

CObject::~CObject()
{
}

uint32_t CObject::GetID() const
{
	return m_id;
}

bool CObject::IsActive() const
{
	return m_active;
}

void CObject::SetActive(bool active)
{
	m_active = active;
}

const std::string& CObject::GetName() const
{
	return m_name;
}

void CObject::SetName(const std::string& name)
{
	m_name = name;
}

void CObject::SetPosition(glm::vec3& pos)
{
	m_position = pos;
}

const glm::vec3& CObject::GetPosition() const
{
	return m_position;
}

glm::vec3& CObject::GetPosition()
{
	return m_position;
}

void CObject::CreateSound3D(Sound::CSound3D* c)
{
	if (m_sound3d)
		delete m_sound3d;

	m_sound3d = c;
}

void CObject::DestroySound3D()
{
	if (m_sound3d)
	{
		delete m_sound3d;
		m_sound3d = nullptr;
	}
}

Sound::CSound3D* CObject::GetSound3D() const
{
	return m_sound3d;
}

void CObject::Update()
{
	if (m_sound3d)
		m_sound3d->Update(System::CTimer::Instance().GetDelta(), toFMOD_VECTOR(m_position));

	if (m_mesh)
		m_mesh->Draw();
}

void CObject::CreateMesh(Graphics::CMesh* c)
{
	if (m_mesh)
		delete m_mesh;

	m_mesh = c;
}

void CObject::DestroyMesh()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
}

Graphics::CMesh* CObject::GetMesh() const
{
	return m_mesh;
}

void CObject::CreateTexture(Graphics::CTexture* c)
{
	if (m_texture)
		delete m_texture;

	m_texture = c;
}

void CObject::DestroyTexture()
{
	if (m_texture)
	{
		delete m_texture;
		m_texture = nullptr;
	}
}

Graphics::CTexture* CObject::GetTexture() const
{
	return m_texture;
}