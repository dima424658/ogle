#include <scene/scene.hpp>

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
		/*glm::mat4 model = glm::translate(glm::mat4(1.0f), el.GetPosition());
		if (el.GetTexture())
		{
			el.GetTexture()->Use(GL_TEXTURE0);
			shader.GetShader().Set("texture_diffuse1", 0);
		}

		shader.GetShader().Set("model", model);
		*/
		el.Update();

		if(el.GetCamera())
			m_cameraQueue.push_back(&el);
			
		if(el.GetMesh())
			m_drawQueue.push_back(&el);
		
	}
}

void CScene::Draw(Graphics::CDeferred& def)
{
	for(auto cameraObject : m_cameraQueue)
	{
		auto camera = cameraObject->GetCamera();
		auto name = std::string("camera##") + std::to_string(cameraObject->GetID());

        static bool show_camera = true;
    	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(FLT_MAX, FLT_MAX));
        if (ImGui::Begin(name.c_str(), &show_camera, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_DockNodeHost))
        {
            camera->SetRenderSize(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40);
			
            if(ImGui::IsWindowHovered())
            {
                if (System::GetInput().IsMousePressed(SDL_BUTTON_MIDDLE))
                {
                    if (System::GetInput().IsKeyPressed(SDL_SCANCODE_LCTRL))
                        camera->UpdateZoom({System::GetInput().GetMouseDelta().first, System::GetInput().GetMouseDelta().second});
                    else if (System::GetInput().IsKeyPressed(SDL_SCANCODE_LSHIFT))
                        camera->UpdatePan({System::GetInput().GetMouseDelta().first, System::GetInput().GetMouseDelta().second});
                    else
                        camera->UpdateRotate({System::GetInput().GetMouseDelta().first, System::GetInput().GetMouseDelta().second});
                }
            }

            def.Prepare();

            //def.GetShader().Set("texture_diffuse1", 0);
            //texture.Use(GL_TEXTURE0);

            //def.GetShader().Set("texture_specular1", 1);
            //textureSpec.Use(GL_TEXTURE1);
            
            def.GetShader().Set("projection", camera->GetProjection());
            def.GetShader().Set("view", camera->GetView());
			
			for(auto mesh : m_drawQueue)
			{
            	def.GetShader().Set("model", glm::translate(glm::mat4(1.0f), mesh->GetPosition()));
				
				if(mesh->GetMaterial())
				{
					def.GetShader().Set("color_diffuse", mesh->GetMaterial()->GetDiffuseColor());
					if(mesh->GetMaterial()->GetDiffuse())
					{
						def.GetShader().Set("has_diffuse", true);
            			def.GetShader().Set("texture_diffuse", 0);
						mesh->GetMaterial()->GetDiffuse()->Use(GL_TEXTURE0);
					}
					else
						def.GetShader().Set("has_diffuse", false);

					def.GetShader().Set("power_specular", mesh->GetMaterial()->GetSpecularPower());
					if(mesh->GetMaterial()->GetSpecular())
					{
						def.GetShader().Set("has_specular", true);
            			def.GetShader().Set("texture_specular", 0);
						mesh->GetMaterial()->GetDiffuse()->Use(GL_TEXTURE1);
					}
					else
						def.GetShader().Set("has_specular", false);
				}
				else
				{
					def.GetShader().Set("has_diffuse", false);
					def.GetShader().Set("has_specular", false);
					def.GetShader().Set("color_diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					def.GetShader().Set("power_specular", 1.0f);
				}
				mesh->GetMesh()->Draw();
			}
			
            camera->Update();
            
            def.Resize(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40);
            GLuint texture = def.DrawToTexture(camera->GetPosition());
            ImGui::Image((void*)(intptr_t)texture, ImVec2(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 40));
        }
        ImGui::End();
	}

	m_cameraQueue.clear();
	m_drawQueue.clear();
}

uint32_t CScene::AddObject(const std::string& name, bool active)
{
	m_objects.push_back(CObject(name, active));
	return m_objects.back().GetID();
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
