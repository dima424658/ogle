#include <graphics/ui/editor.hpp>

using namespace Graphics;

CEditor::CEditor(CScene &scene) : m_scene(scene), m_selected(nullptr)
{
    m_showLog = m_showOverlay = m_showInspector = m_showDock = m_showHierarhy = true;
}

CEditor::~CEditor()
{
}

void CEditor::Update()
{
    ShowDockSpace(&m_showDock);

    if (m_showOverlay)
        ShowFPS(&m_showOverlay);

    if (m_showHierarhy)
        m_selected = m_scene.GetObject(ShowSceneHierarhy(&m_showHierarhy, m_scene));

    if (m_showInspector)
        ShowObjectInspector(&m_showInspector, m_selected);

    if (m_showLog)
        ShowLog(&m_showLog);
}

void CEditor::ShowDockSpace(bool *p_open)
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Engine", p_open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Engine"))
        {
            if (ImGui::MenuItem("New scene"))
                    m_scene.DestroyScene();

            if (ImGui::MenuItem("Load scene.."))
                GetUI().CreateWindow("Load scene..", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [&](const std::string &path)
                {
                    m_scene.LoadScene(path);
                }));

            if (ImGui::MenuItem("Save scene.."))
                GetUI().CreateWindow("Save scene..", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [&](const std::string &path)
                {
                    m_scene.SaveScene(path);
                }));

            ImGui::Separator();
            if (ImGui::MenuItem("Close", nullptr, false, p_open != nullptr))
                System::Exit();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Show log window", "", m_showLog))
                m_showLog = !m_showLog;

            if (ImGui::MenuItem("Show overlay", "", m_showOverlay))
                m_showOverlay = !m_showOverlay;

            if (ImGui::MenuItem("Show object inspector", "", m_showInspector))
                m_showInspector = !m_showInspector;

            if (ImGui::MenuItem("Show schene hierarhy", "", m_showHierarhy))
                m_showHierarhy = !m_showHierarhy;

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::End();
}

void CEditor::ShowExplorerRec(std::filesystem::path path, std::filesystem::path &selected)
{
    std::filesystem::directory_iterator dir(path);

    for (auto &o : dir)
    {
        if (o.is_directory())
        {
            if (ImGui::TreeNode(o.path().filename().string().c_str()))
            {
                ShowExplorerRec(o.path(), selected);
                ImGui::TreePop();
            }
        }
        else if (ImGui::Selectable(o.path().filename().string().c_str(), selected == o.path()))
            selected = o.path();
    }
}

bool CEditor::ShowFileExplorer(const std::string &name, std::function<void(const std::string &)> openCb)
{
    bool p_open = true;

    static std::filesystem::path selected = "";

    ImGui::SetNextWindowSize(ImVec2(500, 440));

    ImGui::OpenPopup("Open File##");

    if (ImGui::BeginPopupModal("Open File##", nullptr, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::BeginChild("top_pane", ImVec2(0, ImGui::GetWindowHeight() - 70), true);

        if (ImGui::TreeNode("."))
        {
            std::filesystem::path path = ".";

            ShowExplorerRec(path, selected);

            ImGui::TreePop();
        }

        ImGui::EndChild();

        if (ImGui::Button("Open"))
        {
            p_open = false;
            openCb(selected.relative_path().string());
        }

        ImGui::SameLine();

        if (ImGui::Button("Exit"))
            p_open = false;

        ImGui::EndPopup();
    }

    return !p_open;
}

uint32_t CEditor::ShowSceneHierarhy(bool *p_open, CScene &scene)
{
    static uint32_t selected = 0;

    if (ImGui::Begin("Scene Hierarhy", p_open))
    {
        // Left

        ImGui::BeginChild("left pane", ImVec2(0, ImGui::GetWindowHeight() - 65), false);
        for (auto &o : scene.GetObjects())
        {
            if (ImGui::Selectable((o.GetName() + "##" + std::to_string(o.GetID())).c_str(), selected == o.GetID()))
                selected = o.GetID();
        }
        ImGui::EndChild();

        if (ImGui::Button("Add"))
            scene.AddObject("default", true);

        ImGui::SameLine();

        if (ImGui::Button("Delete"))
            scene.DeleteObject(selected);
    }

    ImGui::End();

    return selected;
}

void CEditor::ShowObjectInspector(bool *p_open, CObject *object)
{
    static std::string buffer;

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Object Inspector", p_open))
    {
        ImGui::End();
        return;
    }

    if (object == nullptr)
    {
        ImGui::TextWrapped("Please select an object.");
        ImGui::End();
        return;
    }

    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
    ImGui::Text("ID: %u", object->GetID());

    buffer = object->GetName();
    if (ImGui::InputText("Name", &buffer))
        object->SetName(buffer);

    ImGui::DragFloat3("Position", glm::value_ptr(object->GetPosition()), 0.05f, 0.0f, 0.0f, "%.3f");
    ImGui::DragFloat3("Rotation", glm::value_ptr(object->GetRotation()), 0.5f, 0.0f, 0.0f, "%.3f");
    ImGui::DragFloat3("Scale", glm::value_ptr(object->GetScale()), 0.05f, 0.0f, 0.0f, "%.3f");

    if (object->GetSound3D())
        if (ImGui::CollapsingHeader("Sound3D", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Sound name: %s", object->GetSound3D()->GetName().c_str());

            bool playing = object->GetSound3D()->IsPlaying();
            if (ImGui::Checkbox("Playing", &playing))
                playing ? object->GetSound3D()->Play() : object->GetSound3D()->Pause();

            float volume = object->GetSound3D()->GetVolume();

            if (ImGui::DragFloat("Volume", &volume, 0.1f, 0.0f, 100.0f, "%.1f"))
                object->GetSound3D()->SetVolume(volume);
        }

    if (object->GetMesh())
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static char mesh_buffer[128] = {'\0'};

            if (object->GetMesh()->isLoaded())
            {
                ImGui::Text("Object name: %s", object->GetMesh()->GetName().c_str());
                ImGui::Text("Path to object: %s", object->GetMesh()->GetPath().c_str());
            }

            if (ImGui::Button("Load Model.."))
                ImGui::OpenPopup("load_model_popup");

            if (ImGui::BeginPopup("load_model_popup"))
            {
                ImGui::InputText("Name", mesh_buffer, IM_ARRAYSIZE(mesh_buffer));
                ImGui::SameLine();
                if (ImGui::Button("..."))
                {
                    GetUI().CreateWindow("asd", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [object](const std::string &path)
                    {
                        strncpy(mesh_buffer, path.c_str(), 128);
                        object->GetMesh()->LoadModel(mesh_buffer);
                    }));
                }

                if (ImGui::Button("Load"))
                    object->GetMesh()->LoadModel(mesh_buffer);

                ImGui::EndPopup();
            }
        }


    if (object->GetLight())
    {
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto color = object->GetLight()->GetColor();
            auto linear = object->GetLight()->GetLinear();
            auto quadratic = object->GetLight()->GetQuadratic();

            if (ImGui::ColorEdit3("Color", &color[0], ImGuiColorEditFlags_AlphaBar))
                object->GetLight()->SetColor(color);

            if(ImGui::DragFloat("Linear value", &linear, 0.1f, 0.0f, 1000.0f, "%.3f"))
                object->GetLight()->SetLinear(linear);

            if(ImGui::DragFloat("Quadratic value", &quadratic, 0.1f, 0.0f, 1000.0f, "%.3f"))
                object->GetLight()->SetQuadratic(quadratic);
        }
    }

    if (object->GetMaterial())
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::BeginChild("texture_child");
            static char texture_buffer[128] = {'\0'};

            auto color = object->GetMaterial()->GetDiffuseColor();
            if (ImGui::ColorEdit4("Diffuse Color", color, ImGuiColorEditFlags_AlphaBar))
                object->GetMaterial()->SetDiffuseColor(color);

            float spec = object->GetMaterial()->GetSpecularPower() * 100;
            if (ImGui::DragFloat("Specular power", &spec, 1.0f, 0.0f, 100.0f, "%.0f"))
                object->GetMaterial()->SetSpecularPower(spec / 100);

            if (ImGui::Button("Load Diffuse.."))
                ImGui::OpenPopup("load_diffuse_popup");

            ImGui::SameLine();

            if (ImGui::Button("Load Specular.."))
                ImGui::OpenPopup("load_specular_popup");

            ImGui::SameLine();

            if (ImGui::Button("Load Normal.."))
                ImGui::OpenPopup("load_normal_popup");

            if (object->GetMaterial()->GetDiffuse() || object->GetMaterial()->GetNormal() || object->GetMaterial()->GetSpecular())
            {
                if (ImGui::CollapsingHeader("Texture preview"))
                {
                    if (object->GetMaterial()->GetDiffuse())
                    {
                        ImGui::Image((void *)(intptr_t)object->GetMaterial()->GetDiffuse()->GetID(), ImVec2(ImGui::GetWindowWidth() / 4.5, ImGui::GetWindowWidth() / 4.5));
                        ImGui::SameLine();
                    }

                    if (object->GetMaterial()->GetSpecular())
                    {
                        ImGui::Image((void *)(intptr_t)object->GetMaterial()->GetSpecular()->GetID(), ImVec2(ImGui::GetWindowWidth() / 4.5, ImGui::GetWindowWidth() / 4.5));
                        ImGui::SameLine();
                    }

                    if (object->GetMaterial()->GetNormal())
                        ImGui::Image((void *)(intptr_t)object->GetMaterial()->GetNormal()->GetID(), ImVec2(ImGui::GetWindowWidth() / 4.5, ImGui::GetWindowWidth() / 4.5));
                }
            }

            if (ImGui::BeginPopup("load_diffuse_popup"))
            {
                ImGui::InputText("Name", texture_buffer, IM_ARRAYSIZE(texture_buffer));
                ImGui::SameLine();
                if (ImGui::Button("..."))
                {
                    GetUI().CreateWindow("Load diffuse", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [object](const std::string &path)
                    {
                        strncpy(texture_buffer, path.c_str(), 128);
                        object->GetMaterial()->LoadDiffuse(texture_buffer);
                    }));
                }

                if (ImGui::Button("Load"))
                    object->GetMaterial()->LoadDiffuse(texture_buffer);

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("load_specular_popup"))
            {
                ImGui::InputText("Name", texture_buffer, IM_ARRAYSIZE(texture_buffer));
                ImGui::SameLine();
                if (ImGui::Button("..."))
                {
                    GetUI().CreateWindow("Load specular", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [object](const std::string &path)
                    {
                        strncpy(texture_buffer, path.c_str(), 128);
                        object->GetMaterial()->LoadSpecular(texture_buffer);
                    }));
                }

                if (ImGui::Button("Load"))
                    object->GetMaterial()->LoadSpecular(texture_buffer);

                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("load_normal_popup"))
            {
                ImGui::InputText("Name", texture_buffer, IM_ARRAYSIZE(texture_buffer));
                ImGui::SameLine();
                if (ImGui::Button("..."))
                {
                    GetUI().CreateWindow("Load normal", std::bind(&CEditor::ShowFileExplorer, std::placeholders::_1, [object](const std::string &path)
                    {
                        strncpy(texture_buffer, path.c_str(), 128);
                        object->GetMaterial()->LoadNormal(texture_buffer);
                    }));
                }

                if (ImGui::Button("Load"))
                    object->GetMaterial()->LoadNormal(texture_buffer);

                ImGui::EndPopup();
            }
            ImGui::EndChild();
        }

    ImGui::EndChild();

    const char *component_names[] = {"Sound3D", "Mesh", "Material", "Point light"};

    if (ImGui::Button("Add component.."))
        ImGui::OpenPopup("component_popup");

    if (ImGui::BeginPopup("component_popup"))
    {
        for (int i = 0; i < IM_ARRAYSIZE(component_names); i++)
            if (ImGui::Selectable(component_names[i]))
            {
                if (i == 0 && !object->GetSound3D())
                    object->CreateSound3D(new Sound::CSound3D("data/test.mp3", false));

                if (i == 1 && !object->GetMesh())
                    object->CreateMesh(new Graphics::CMesh());

                if (i == 2 && !object->GetMaterial())
                    object->CreateMaterial(new Graphics::CMaterial());

                if (i == 3 && !object->GetLight())
                    object->CreateLight(new Graphics::CLight());
            }
        ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
    }

    ImGui::End();
}

void CEditor::ShowFPS(bool *p_open)
{
    // FIXME-VIEWPORT: Select a default viewport
    const float DISTANCE = 10.0f;
    static int corner = 3;

    if (corner != -1)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos(); // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
        window_flags |= ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("Overlay", p_open, window_flags))
    {
        ImGui::Text("FPS: %d", static_cast<int>(1000.0f / System::GetTimer().GetDelta()));
        /*ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");*/
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, corner == -1))
                corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0))
                corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1))
                corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2))
                corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3))
                corner = 3;
            if (p_open && ImGui::MenuItem("Close"))
                *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void CEditor::ShowLog(bool *p_open)
{
    static ImVec4 errorColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
    static ImVec4 warningColor = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);

    static bool auto_scroll = true;
    static bool show_logs = true;
    static bool show_errors = true;
    static bool show_warnings = true;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Log", p_open))
    {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Show logs", &show_logs);
        ImGui::Checkbox("Show warnings", &show_warnings);
        ImGui::Checkbox("Show errors", &show_errors);

        ImGui::Checkbox("Auto-scroll", &auto_scroll);

        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    //ImGui::SameLine();
    //Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        System::CLog::ClearMessages();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    auto messages = System::CLog::GetMessages();

    for (size_t i = 0; i < messages.size(); i++)
    {
        auto text = std::to_string(std::get<1>(messages[i]) / 1000) + " " + std::get<2>(messages[i]);

        if (std::get<0>(messages[i]) == System::CLog::SType::Error && show_errors)
            ImGui::TextColored(errorColor, text.c_str());

        if (std::get<0>(messages[i]) == System::CLog::SType::Warning && show_warnings)
            ImGui::TextColored(warningColor, text.c_str());

        if (std::get<0>(messages[i]) == System::CLog::SType::Log && show_logs)
            ImGui::TextWrapped(text.c_str());
    }

    ImGui::PopStyleVar();

    if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}