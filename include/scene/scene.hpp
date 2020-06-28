#pragma once

#undef GetObject

#include <scene/object.hpp>
#include <graphics/deferred.hpp>
#include <list>

#include <glm/gtc/matrix_transform.hpp>

class CScene
{
public:
	CScene();
	~CScene();

	void Update(const Graphics::CDeferred& shader);

	uint32_t AddObject(const std::string& name, bool active = true);

	void DeleteObject(const std::string_view& name);
	void DeleteObject(std::list<CObject>::const_iterator it);
	void DeleteObject(uint32_t id);

	CObject* GetObject(const std::string_view& name);
	CObject* GetObject(std::list<CObject>::iterator it);
	CObject* GetObject(uint32_t id);

	const std::list<CObject>& GetObjects() const;
private:
	std::list<CObject> m_objects;
};