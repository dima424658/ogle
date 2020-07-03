#pragma once

#undef GetObject

#include <scene/object.hpp>
#include <graphics/deferred.hpp>
#include <graphics/ui.hpp>
#include <system/input.hpp>

#include <list>
#include <algorithm>
#include <queue>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#undef Bool
#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

template <typename Encoding>
struct GenericRapidStream
{
	typedef typename Encoding::Ch Ch;

	GenericRapidStream(const std::basic_string_view<Ch> path) : outstream(path.data()) {}
	~GenericRapidStream()
	{
		outstream.close();
	}
	void Put(Ch ch)
	{
		outstream.put(ch);
	}
	void Flush() {}

	std::basic_ofstream<Ch> outstream;
};

typedef GenericRapidStream<rapidjson::UTF8<>> RapidStream;

class CScene
{
public:
	CScene();
	~CScene();

	void Update();
	void Draw(Graphics::CDeferred &shader);

	uint32_t AddObject(const std::string &name, bool active = true);

	void DeleteObject(const std::string_view &name);
	void DeleteObject(std::list<CObject>::const_iterator it);
	void DeleteObject(uint32_t id);

	CObject *GetObject(const std::string_view &name);
	CObject *GetObject(std::list<CObject>::iterator it);
	CObject *GetObject(uint32_t id);

	const std::list<CObject> &GetObjects() const;

	void DestroyScene();
	void LoadScene(std::string_view path);
	void SaveScene(std::string_view path);

private:
	template <size_t I>
	glm::vec<I, float, glm::defaultp> LoadVec(const rapidjson::Value &json)
	{
		glm::vec<I, float, glm::defaultp> result = glm::vec<I, float, glm::defaultp>(0.0f);

		const auto &array = json.GetArray();
		if (array.Size() >= I)
		{
			for (size_t i = 0; i < I; i++)
				result[i] = array[i].GetFloat();
		}
		else
			System::Warning() << "Incorrect vec" << std::to_string(I) << " value.";

		return result;
	}

	template <size_t I>
	void SaveVec(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator, const glm::vec<I, float, glm::defaultp> &vector)
	{
		if (json.IsArray())
			json.GetArray().Clear();
		else
			json.SetArray();

		for (size_t i = 0; i < I; i++)
		{
			rapidjson::Value val;
			val.SetFloat(vector[i]);
			json.PushBack(std::move(val), allocator);
		}
	}

	Graphics::CMaterial LoadMaterial(const rapidjson::Value &json);
	void SaveMaterial(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator, const Graphics::CMaterial &mat);

private:
	std::vector<CObject *> m_drawQueue;
	std::vector<CObject *> m_lightQueue;
	CCamera m_camera;

	std::list<CObject> m_objects;
};