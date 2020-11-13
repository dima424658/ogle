#include <scene/scene.hpp>

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Update()
{
	for (auto &el : m_objects)
	{
		el.Update();

		if (el.GetMesh())
			m_drawQueue.push_back(&el);

		if (el.GetLight())
			m_lightQueue.push_back(&el); // было бы круто не мониторить каждый кадр
										 // очереди и динамически их обновлять...
	}
}

void CScene::Draw(Graphics::CDeferred &def)
{
	static bool show_camera = true;
	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(FLT_MAX, FLT_MAX));
	if (ImGui::Begin("Scene view", &show_camera, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_DockNodeHost))
	{
		m_camera.SetRenderSize(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40);

		if (ImGui::IsWindowHovered())
		{
			if (System::GetInput().IsMousePressed(SDL_BUTTON_MIDDLE))
			{
				if (System::GetInput().IsKeyPressed(SDL_SCANCODE_LCTRL))
					m_camera.UpdateZoom(System::GetInput().GetMouseDelta().second);
				else
					m_camera.UpdatePan({System::GetInput().GetMouseDelta().first, System::GetInput().GetMouseDelta().second});
			}

			if (System::GetInput().IsMousePressed(SDL_BUTTON_RIGHT))
				m_camera.UpdateRotate({System::GetInput().GetMouseDelta().first, System::GetInput().GetMouseDelta().second});
		}

		def.Prepare();
		
		for (auto mesh : m_drawQueue)
		{
			static glm::mat4 modelMat;
			modelMat = glm::translate(glm::mat4(1.0f), mesh->GetPosition());
			modelMat = modelMat * glm::eulerAngleXYZ(glm::radians(mesh->GetRotation().x),
													 glm::radians(mesh->GetRotation().y), glm::radians(mesh->GetRotation().z));
			modelMat = glm::scale(modelMat, mesh->GetScale());

			def.GetGShader().Set("model", modelMat);

			if (mesh->GetMaterial())
			{
				def.GetGShader().Set("color_diffuse", mesh->GetMaterial()->GetDiffuseColor());
				if (mesh->GetMaterial()->GetDiffuse())
				{
					def.GetGShader().Set("has_diffuse", true);
					def.GetGShader().Set("texture_diffuse", 0);
					mesh->GetMaterial()->GetDiffuse()->Use(GL_TEXTURE0);
				}
				else
					def.GetGShader().Set("has_diffuse", false);

				def.GetGShader().Set("power_specular", mesh->GetMaterial()->GetSpecularPower());
				if (mesh->GetMaterial()->GetSpecular())
				{
					def.GetGShader().Set("has_specular", true);
					def.GetGShader().Set("texture_specular", 1);
					mesh->GetMaterial()->GetDiffuse()->Use(GL_TEXTURE1);
				}
				else
					def.GetGShader().Set("has_specular", false);
				
				if(mesh->GetMaterial()->GetNormal())
				{
					def.GetGShader().Set("has_normal", true);
					def.GetGShader().Set("texture_normal", 2);
					mesh->GetMaterial()->GetNormal()->Use(GL_TEXTURE2);

				}
				else
					def.GetGShader().Set("has_normal", false);
			}
			else
			{
				def.GetGShader().Set("has_diffuse", false);
				def.GetGShader().Set("has_specular", false);
				def.GetGShader().Set("has_normal", false);
				def.GetGShader().Set("color_diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				def.GetGShader().Set("power_specular", 1.0f);
			}

			auto oID = mesh->GetID();

			glm::vec4 color;

			color[0] = oID % 256;
			color[1] = (oID >> 8) % 256;
			color[2] = (oID >> 16) % 256;
			color[3] = (oID >> 24) % 256;
			color /= 255.0f;

			def.GetGShader().Set("oID", color);

			mesh->GetMesh()->Draw();
		}

		m_camera.Update();

		def.GetGShader().Set("projection", m_camera.GetProjection());
		def.GetGShader().Set("view", m_camera.GetView());

		def.GetDiffuseShader().Use();
		size_t lightSize = std::min(m_lightQueue.size(), 16UL);
		def.GetDiffuseShader().Set("lightCount", static_cast<GLint>(lightSize));
		size_t i = 0;

 //   glEnable(GL_DEBUG_OUTPUT); // TODO
   // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		for(auto light : m_lightQueue)
		{
			if(i >= lightSize)
				break;

            def.GetDiffuseShader().Set("lights[" + std::to_string(i) + "].position", light->GetPosition());
            def.GetDiffuseShader().Set("lights[" + std::to_string(i) + "].color", light->GetLight()->GetColor());
            def.GetDiffuseShader().Set("lights[" + std::to_string(i) + "].linear", light->GetLight()->GetLinear());
            def.GetDiffuseShader().Set("lights[" + std::to_string(i) + "].quadratic", light->GetLight()->GetQuadratic());
			
			i++;
		}

		def.Resize(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40);
		GLuint texture = def.DrawToTexture(m_camera.GetPosition());
		glBindFramebuffer(GL_FRAMEBUFFER, def.GetFinalbuffer());
		glDepthFunc(GL_ALWAYS);

		static Graphics::CTexture testTexture("data/light.png");

		for(auto light : m_lightQueue)
		{
		Graphics::CSprite sprite;

		static glm::mat4 spriteMat;
		spriteMat = glm::translate(glm::mat4(1.0f), light->GetPosition());

		sprite.Render(glm::vec2(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40), glm::vec2(512, 512),
		testTexture.GetID(), light->GetLight()->GetColor(), spriteMat, m_camera.GetView(), m_camera.GetProjection());

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (ImGui::IsWindowHovered())
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				ImVec2 pos = ImGui::GetMousePos();
				pos.x -= ImGui::GetWindowPos().x + 8;
				pos.y -= ImGui::GetWindowPos().y + 30;

				auto object = GetObject(def.GetObjectID(pos.x, pos.y));
				if(object != nullptr)
				{
					System::Log() << "пикнут " << object->GetName();
				}
			}
		}

		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), ImVec2(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40));
	}
	ImGui::End();

	m_drawQueue.clear();
	m_lightQueue.clear();
}

uint32_t CScene::AddObject(const std::string &name, bool active)
{
	m_objects.push_back(CObject(name, active));
	return m_objects.back().GetID();
}

void CScene::DeleteObject(const std::string_view &name)
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
		if (it->GetID() == id)
		{
			auto oldit = it;
			it++;
			m_objects.erase(oldit);
			continue;
		}

		it++;
	}
}

const CCamera& CScene::GetActiveCamera() const
{
	return m_camera;
}

void CScene::DeleteObject(std::list<CObject>::const_iterator it)
{
	m_objects.erase(it);
}

CObject *CScene::GetObject(const std::string_view &name)
{
	// TODO delete multiple times
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [name](const CObject &o) -> bool {
		return !o.GetName().compare(name.data());
	});

	if (it != m_objects.end())
	{
		CObject &a = *it;
		return &a;
	}

	return nullptr;
}

CObject *CScene::GetObject(uint32_t id)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [id](const CObject &o) -> bool {
		return o.GetID() == id;
	});

	if (it != m_objects.end())
	{
		CObject &a = *it;
		return &a;
	}

	return nullptr;
}

CObject *CScene::GetObject(std::list<CObject>::iterator it)
{
	if (it != m_objects.end())
	{
		CObject &a = *it;
		return &a;
	}

	return nullptr;
}

const std::list<CObject> &CScene::GetObjects() const
{
	return m_objects;
}

void CScene::DestroyScene()
{
	m_objects.clear();
	m_drawQueue.clear();
	m_lightQueue.clear();
	m_camera = CCamera();
}

void CScene::LoadScene(std::string_view path)
{
	System::Log() << "Loading scene: \"" << path << "\"";

	rapidjson::Document document;
	std::ifstream file(path.data());
	if (!file.is_open())
	{
		System::Warning() << "Failed to load scene: \"" << path << "\"";
		return;
	}

	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	rapidjson::ParseResult result = document.Parse(data.c_str());
	if (!result)
	{
		System::Warning() << "Failed to parse json scene file: \"" << path << "\"";
		return;
	}

	file.close();
	//data.~basic_string();

	DestroyScene();

	if (!document.HasMember("scene") || !document["scene"].IsArray())
	{
		System::Warning() << "Failed to load scene: \"" << path << "\". File doesn't contain scene array.";
		return;
	}

	for (auto &object : document["scene"].GetArray())
	{
		if (!object.HasMember("Name") || !object["Name"].IsString())
		{
			System::Warning() << "Object doesn't have name, skipping: \"" << path << "\"";
			continue;
		}

		if (!object.HasMember("ID") || !object["ID"].IsInt())
		{
			System::Warning() << object["Name"].GetString() << " doesn't have ID, skipping: \"" << path << "\"";
			continue;
		}

		m_objects.push_back(CObject(object["Name"].GetString(), true, object["ID"].GetUint()));

		if (object.HasMember("Mesh"))
			if (object["Mesh"].HasMember("Path"))
			{
				m_objects.back().CreateMesh(new Graphics::CMesh(object["Mesh"]["Path"].GetString()));
			}

		if (object.HasMember("Light"))
			if (object["Light"].IsObject())
			{
				glm::vec3 color(1.0f);

				if (object["Light"].HasMember("Color"))
					if (object["Light"]["Color"].IsArray())
						color = LoadVec<3>(object["Light"]["Color"]);

				m_objects.back().CreateLight(new Graphics::CLight(color));

				if (object["Light"].HasMember("Linear"))
					if (object["Light"]["Linear"].IsFloat())
						m_objects.back().GetLight()->SetLinear(object["Light"]["Linear"].GetFloat());

				if (object["Light"].HasMember("Quadratic"))
					if (object["Light"]["Quadratic"].IsFloat())
						m_objects.back().GetLight()->SetQuadratic(object["Light"]["Quadratic"].GetFloat());
			}

		if (object.HasMember("Position"))
			if (object["Position"].IsArray())
				m_objects.back().SetPosition(LoadVec<3>(object["Position"]));

		
		if (object.HasMember("Rotation"))
			if (object["Rotation"].IsArray())
				m_objects.back().SetRotation(LoadVec<3>(object["Rotation"]));

		
		if (object.HasMember("Scale"))
			if (object["Scale"].IsArray())
				m_objects.back().SetScale(LoadVec<3>(object["Scale"]));

		if (object.HasMember("Material"))
			m_objects.back().CreateMaterial(new Graphics::CMaterial(LoadMaterial(object["Material"])));

		if (object.HasMember("Camera"))
		{
		}
	}

	System::Log() << "Loading successful.";
}

void CScene::SaveScene(std::string_view path)
{
	rapidjson::Document document;
	document.SetObject();

	document.AddMember("scene", rapidjson::kArrayType, document.GetAllocator());

	auto &json = document["scene"];
	for (auto &object : m_objects)
	{
		rapidjson::Value val;
		val.SetObject();
		val.AddMember("Name", rapidjson::kStringType, document.GetAllocator());
		val["Name"].SetString(rapidjson::StringRef(object.GetName()));

		val.AddMember("ID", rapidjson::kNumberType, document.GetAllocator());
		val["ID"].SetUint(object.GetID());

		val.AddMember("Position", rapidjson::kArrayType, document.GetAllocator());
		SaveVec<3>(val["Position"], document.GetAllocator(), object.GetPosition());
		
		val.AddMember("Rotation", rapidjson::kArrayType, document.GetAllocator());
		SaveVec<3>(val["Rotation"], document.GetAllocator(), object.GetRotation());
		
		val.AddMember("Scale", rapidjson::kArrayType, document.GetAllocator());
		SaveVec<3>(val["Scale"], document.GetAllocator(), object.GetScale());

		if (object.GetMaterial())
		{
			val.AddMember("Material", rapidjson::kObjectType, document.GetAllocator());
			SaveMaterial(val["Material"], document.GetAllocator(), *object.GetMaterial());
		}

		if (object.GetMesh())
		{

			val.AddMember("Mesh", rapidjson::kObjectType, document.GetAllocator());
			val["Mesh"].AddMember("Path", rapidjson::kStringType, document.GetAllocator());
			val["Mesh"]["Path"].SetString(rapidjson::StringRef(object.GetMesh()->GetPath()));
		}

		if (object.GetLight())
		{
			val.AddMember("Light", rapidjson::kObjectType, document.GetAllocator());
			val["Light"].AddMember("Color", rapidjson::kArrayType, document.GetAllocator());
			SaveVec<3>(val["Light"]["Color"], document.GetAllocator(), object.GetLight()->GetColor());
			
			val["Light"].AddMember("Linear", rapidjson::kNumberType, document.GetAllocator());
			val["Light"]["Linear"].SetFloat(object.GetLight()->GetLinear());

			val["Light"].AddMember("Quadratic", rapidjson::kNumberType, document.GetAllocator());
			val["Light"]["Quadratic"].SetFloat(object.GetLight()->GetQuadratic());
		}


		//rapidjson::StringRef(object.GetName()), rapidjson::kObjectType, document.GetAllocator()
		auto &el = json.PushBack(std::move(val), document.GetAllocator());
	}

	RapidStream buffer(path);
	rapidjson::StringBuffer A;
	rapidjson::Writer<RapidStream> writer(buffer);
	document.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
}

Graphics::CMaterial CScene::LoadMaterial(const rapidjson::GenericValue<rapidjson::UTF8<>> &json)
{
	Graphics::CMaterial result;

	if (json.HasMember("DiffusePath"))
		if (json["DiffusePath"].IsString())
			result.LoadDiffuse(json["DiffusePath"].GetString());

	if (json.HasMember("SpecularPath"))
		if (json["SpecularPath"].IsString())
			result.LoadSpecular(json["SpecularPath"].GetString());

	if (json.HasMember("NormalPath"))
		if (json["NormalPath"].IsString())
			result.LoadNormal(json["NormalPath"].GetString());

	if (json.HasMember("DiffuseColor"))
		if (json["DiffuseColor"].IsArray())
			result.SetDiffuseColor(LoadVec<3>(json["DiffuseColor"]));

	if (json.HasMember("SpecularPower"))
		if (json["SpecularPower"].IsFloat())
			result.SetSpecularPower(json["SpecularPower"].GetFloat());

	return result;
}

void CScene::SaveMaterial(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator, const Graphics::CMaterial &mat)
{
	json.SetObject();

	json.AddMember("DiffuseColor", rapidjson::kArrayType, allocator);
	SaveVec<3>(json["DiffuseColor"], allocator, mat.GetDiffuseColor());

	json.AddMember("SpecularPower", rapidjson::kNumberType, allocator);
	json["SpecularPower"].SetFloat(mat.GetSpecularPower());

	if (mat.HasDiffuse())
	{
		json.AddMember("DiffusePath", rapidjson::kStringType, allocator);
		json["DiffusePath"].SetString(rapidjson::StringRef(mat.GetDiffuse()->GetPath()));
	}

	if (mat.HasNormal())
	{
		json.AddMember("NormalPath", rapidjson::kStringType, allocator);
		json["NormalPath"].SetString(rapidjson::StringRef(mat.GetNormal()->GetPath()));
	}

	if (mat.HasSpecular())
	{
		json.AddMember("SpecularPath", rapidjson::kStringType, allocator);
		json["SpecularPath"].SetString(rapidjson::StringRef(mat.GetSpecular()->GetPath()));
	}
}