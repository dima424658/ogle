#include <scene/object.hpp>

CObject::CObject(const std::string& name, bool active, uint32_t id) : m_id(id), m_position(0.0f),  m_name(name), m_active(active)
{
	static std::random_device rd;
	static std::mt19937_64 generator = std::mt19937_64(rd());
	static std::uniform_int_distribution<uint32_t> uid;

	if(m_id == 0)
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

void CObject::CreateMaterial(Graphics::CMaterial* c)
{
	if (m_material)
		delete m_material;

	m_material = c;
}

void CObject::DestroyMaterial()
{
	if (m_material)
	{
		delete m_material;
		m_material = nullptr;
	}
}

Graphics::CMaterial* CObject::GetMaterial() const
{
	return m_material;
}

void CObject::CreateCamera(CCamera* c)
{
	if (m_camera)
		delete m_camera;

	m_camera = c;
}

void CObject::DestroyCamera()
{
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}

CCamera* CObject::GetCamera() const
{
	return m_camera;
}
