#include "..\..\include\scene\scene.hpp"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Update()
{
	for (auto& el : m_objects)
	{
		el.Update();
	}
}

void CScene::AddObject(const std::string& name, bool active)
{
	m_objects.push_back(CObject(name, active));
}

void CScene::DeleteObject(const std::string_view& name)
{
	auto it = m_objects.begin();
	while (it != m_objects.end())
	{
		if (!it->GetName().compare(name))
		{
			auto oldit = it;
			it++;
			m_objects.erase(oldit);
			continue;
		}

		it++;
	}
}

void CScene::DeleteObject(uint32_t id)
{
	auto it = m_objects.begin();
	while (it != m_objects.end())
	{
		if (!it->GetID() == id)
		{
			auto oldit = it;
			it++;
			m_objects.erase(oldit);
			continue;
		}

		it++;
	}
}

void CScene::DeleteObject(std::list<CObject>::const_iterator it)
{
	m_objects.erase(it);
}

CObject* CScene::GetObject(const std::string_view& name)
{
	// TODO delete multiple times
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [name](const CObject& o) -> bool
		{
			return !o.GetName().compare(name.data());
		});

	if (it != m_objects.end())
	{
		CObject& a = *it;
		return &a;
	}

	return nullptr;
}

CObject* CScene::GetObject(uint32_t id)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [id](const CObject& o) -> bool
		{
			return o.GetID() == id;
		});

	if (it != m_objects.end())
	{
		CObject& a = *it;
		return &a;
	}

	return nullptr;
}

CObject* CScene::GetObject(std::list<CObject>::iterator it)
{
	if (it != m_objects.end())
	{
		CObject& a = *it;
		return &a;
	}

	return nullptr;
}

const std::list<CObject>& CScene::GetObjects() const
{
	return m_objects;
}
