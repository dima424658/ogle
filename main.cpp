#include "engine.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <filesystem>
#include <functional>

#include <system/extensions.hpp>

glm::mat4 cameraView;
glm::mat4 cameraProjection;
glm::vec3 cameraPosition = { 15.0f, 15.0f, 15.0f };
glm::vec2 cameraSize = { 1600, 900 };
glm::vec3 cameraLookAt = { 0.0f, 0.0f, 0.0f };
glm::vec3 cameraRotation = { 0.0f, 0.0f, 0.0f };

glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
glm::vec3 cameraRight = { 1.0f, 0.0f, 0.0f };

ImVec2 mouseDelta = { 0.0f, 0.0f };

float sensivity = 0.5f;

void cameraRotateUpdate(ImVec2 delta)
{
    glm::vec3 cameraDir = (cameraPosition - cameraLookAt);

    float cameraLength = glm::length(cameraDir);
    cameraDir = glm::normalize(cameraDir);
    
    float x = glm::radians(delta.x * sensivity);
    float y = glm::radians(delta.y * sensivity);

    glm::vec4 up = glm::vec4(cameraUp * y, 0.0f) * cameraView;
    glm::vec4 right = glm::vec4(cameraRight * x, 0.0f) * cameraView;

    cameraDir += glm::vec3(up + right);
    cameraPosition = glm::normalize(cameraDir) * cameraLength + cameraLookAt;

    cameraProjection = glm::perspective(glm::radians(45.0f), cameraSize.x / cameraSize.y, 0.1f, 100.0f);
    cameraView = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
}


void cameraZoomUpdate(ImVec2 delta)
{
    glm::vec3 cameraDir = (cameraPosition - cameraLookAt);
    cameraPosition = glm::normalize(cameraDir) * (std::max(glm::length(cameraDir) + delta.y * sensivity, 1.0f)) + cameraLookAt;

    cameraProjection = glm::perspective(glm::radians(45.0f), cameraSize.x / cameraSize.y, 0.1f, 100.0f);
    cameraView = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
}

void cameraPanUpdate(ImVec2 delta)
{
    constexpr float fov = glm::radians(45.0f);
    float tanFovX = cameraSize.x / cameraSize.y * glm::tan(fov / 2); // tan of x fov

    glm::vec3 cameraDir = (cameraPosition - cameraLookAt);
    glm::vec3 right = glm::normalize(glm::cross(cameraUp, cameraDir));
    glm::vec3 up = glm::normalize(glm::cross(cameraDir, right));

    delta.y = (delta.y / cameraSize.y) * (2 * glm::length(cameraDir) * glm::tan(fov / 2));
    delta.x = (delta.x / cameraSize.x) * (2 * glm::length(cameraDir) * tanFovX);

    cameraLookAt += up * delta.y - right * delta.x;
    cameraPosition += up * delta.y - right * delta.x;

    cameraProjection = glm::perspective(fov, cameraSize.x / cameraSize.y, 0.1f, 100.0f);
    cameraView = glm::lookAt(cameraPosition, cameraLookAt, cameraUp);
}

void ShowExampleAppDockSpace(bool* p_open)
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Engine", p_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Engine"))
        {
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            ImGui::Separator();
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                std::exit(EXIT_SUCCESS);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::End();
}

void ShowExplorerRec(std::filesystem::path path, std::filesystem::path& selected)
{
    std::filesystem::directory_iterator dir(path);

    for (auto& o : dir)
    {
        if (o.is_directory())
        {
            if (ImGui::TreeNode(o.path().filename().string().c_str()))
            {
                ShowExplorerRec(o.path(), selected);
                ImGui::TreePop();
            }
        }
        else
            if (ImGui::Selectable(o.path().filename().string().c_str(), selected == o.path()))
                selected = o.path();
    }

}

bool ShowFileExplorer(const std::string& name, std::function<void(const std::string&)> openCb)
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

uint32_t ShowSceneHierarhy(bool* p_open, CScene& scene)
{
    static uint32_t selected = 0;

    if (ImGui::Begin("Scene Hierarhy", p_open))
    {
        // Left

        ImGui::BeginChild("left pane", ImVec2(0, ImGui::GetWindowHeight() - 65), false);
        for (auto& o : scene.GetObjects())
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

void ShowObjectInspector(bool* p_open, CObject* object)
{
    static char buffer[128];

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Object Inspector", p_open))
    {

        // Right
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
       // ImGui::Text("MyObject: %u", object->GetID());
        ImGui::Separator();
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Description"))
            {
                if(object == nullptr)
                    ImGui::TextWrapped("Please select an object.");
                else
                {
                    strncpy_s(buffer, object->GetName().c_str(), 128);
                    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
                        object->SetName(std::string(buffer));

                    ImGui::DragFloat3("Position", glm::value_ptr(object->GetPosition()), 0.05f, 0.0f, 0.0f, "%.3f");

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
                            static char mesh_buffer[128] = { '\0' };

                            if (object->GetMesh()->isLoaded())
                            {
                                ImGui::Text("Object name: %s", object->GetMesh()->GetName().c_str());
                            }

                            if (ImGui::Button("Load Model.."))
                                ImGui::OpenPopup("load_model_popup");

                            if (ImGui::BeginPopup("load_model_popup"))
                            {
                                ImGui::InputText("Name", mesh_buffer, IM_ARRAYSIZE(mesh_buffer));
                                ImGui::SameLine();
                                if (ImGui::Button("..."))
                                {
                                    GetUI().CreateWindow("asd", std::bind(&ShowFileExplorer, std::placeholders::_1, [object](const std::string& path)
                                        {
                                            strncpy_s(mesh_buffer, path.c_str(), 128);
                                            object->GetMesh()->LoadModel(mesh_buffer);
                                        }));
                                }

                                if (ImGui::Button("Load"))
                                    object->GetMesh()->LoadModel(mesh_buffer);

                                ImGui::EndPopup();
                            }
                        }

                    if(object->GetTexture())
                        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            ImGui::BeginChild("texture_child");
                            static char texture_buffer[128] = { '\0' };

                            if (ImGui::Button("Load Texture.."))
                                ImGui::OpenPopup("load_texture_popup");

                            if (ImGui::CollapsingHeader("Texture preview"))
                                ImGui::Image((void*)(intptr_t)object->GetTexture()->GetID(), ImVec2(ImGui::GetWindowWidth() / 1.5, ImGui::GetWindowWidth() / 1.5));

                            if (ImGui::BeginPopup("load_texture_popup"))
                            {
                                ImGui::InputText("Name", texture_buffer, IM_ARRAYSIZE(texture_buffer));
                                ImGui::SameLine();
                                if (ImGui::Button("..."))
                                {
                                    GetUI().CreateWindow("asd", std::bind(&ShowFileExplorer, std::placeholders::_1, [object](const std::string& path)
                                        {
                                            strncpy_s(texture_buffer, path.c_str(), 128);
                                            object->GetTexture()->LoadImage(texture_buffer);
                                        }));
                                }

                                if (ImGui::Button("Load"))
                                    object->GetTexture()->LoadImage(texture_buffer);

                                ImGui::EndPopup();
                            }
                            ImGui::EndChild();
                        }
                }
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Details"))
            {
           //     ImGui::Text("ID: %u", selected);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();

        if (object)
        {
            const char* component_names[] = { "Sound3D", "Mesh", "Texture" };

            if (ImGui::Button("Add component.."))
                ImGui::OpenPopup("component_popup");

            if (ImGui::BeginPopup("component_popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(component_names); i++)
                    if (ImGui::Selectable(component_names[i]))
                    {
                        if(i == 0 && !object->GetSound3D())
                            object->CreateSound3D(new Sound::CSound3D("data/test.mp3", false));

                        if (i == 1 && !object->GetMesh())
                            object->CreateMesh(new Graphics::CMesh());

                        if (i == 2 && !object->GetTexture())
                            object->CreateTexture(new Graphics::CTexture());
                    }
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
        }
    }
    ImGui::End();
}

void ShowFPS(bool* p_open)
{
    // FIXME-VIEWPORT: Select a default viewport
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos = viewport->GetWorkPos();   // Instead of using viewport->Pos we use GetWorkPos() to avoid menu bars, if any!
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
        ImGui::Text("FPS: %d", static_cast<int>(1000.0f / GetTimer().GetDelta()));
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) *p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();

}

int main()
{
    try
    {
    GetGraphics();
    GetTimer().SetDelay(10);
    CScene scene;

    Graphics::CTexture texture("data/256.jpeg");
    //Graphics::CMesh mesh("data/cube.obj");
    Graphics::CTexture textureSpec("data/container2_specular.png");
    
    Graphics::CDeferred def(1600, 900);
    
    Sound::CSound2D::RegisterScript();
    Sound::CSound3D::RegisterScript();
        
    Script::CScript script("data/script.as");

    script.PrepareFunction("float calc(float, float)");
    script.GetContext()->SetArgFloat(0, 6.0f);
    script.GetContext()->SetArgFloat(1, 2.0f);
    void* result = script.Execute();

    float a = *(static_cast<float*>(result));
    System::Log() << "Result is " << a;
   // sound.Play();

    Sound::CSoundListener listener;

    GetTimer().Sync();

    auto id = scene.AddObject("cube", true);
    scene.GetObject(id)->CreateMesh(new Graphics::CMesh("data/cube.obj"));
    while(true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            GetUI().ProcessEvent(event);
            if (event.type == SDL_QUIT)
                std::exit(EXIT_SUCCESS);
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) //&& event.window.windowID == SDL_GetWindowID(m_window))
                std::exit(EXIT_SUCCESS);
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    def.Resize(event.window.data1, event.window.data2);
                    cameraSize = { event.window.data1 , event.window.data2 };
                    cameraRotateUpdate({ 0.0f,0.0f });
                }
            }
            SDL_PumpEvents();
        }

        static ImVec2 prev = ImGui::GetMousePos();
        ImVec2 curr = ImGui::GetMousePos();
        mouseDelta = { curr.x - prev.x, curr.y - prev.y };
        prev = curr;
   
        auto kState = SDL_GetKeyboardState(nullptr);
        auto mState = SDL_GetMouseState(nullptr, nullptr);

        if (mState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
            if (kState[SDL_SCANCODE_LCTRL])
                cameraZoomUpdate(mouseDelta);
            else if (kState[SDL_SCANCODE_LSHIFT])
                cameraPanUpdate(mouseDelta);
            else
                cameraRotateUpdate(mouseDelta);
        }

        //listener.Update(GetTimer().GetDelta() * 1000, toFMOD_VECTOR(cameraPosition));
        GetSound().Update();

        GetGraphics().Begin();
        def.Prepare();

        def.GetShader().Set("texture_diffuse1", 0);
        texture.Use(GL_TEXTURE0);

        def.GetShader().Set("texture_specular1", 1);
        textureSpec.Use(GL_TEXTURE1);
        
        def.GetShader().Set("projection", cameraProjection);
        def.GetShader().Set("view", cameraView);
        def.GetShader().Set("model", glm::mat4(1.0f));
        //mesh.Draw();

        scene.Update(def);

        def.Draw(cameraPosition);

        GetUI().NewFrame(GetTimer().GetDelta() / 1000);
        static bool show_demo_window = true;
        ShowExampleAppDockSpace(&show_demo_window);

        static bool show_scenehier = true;
        uint32_t id = ShowSceneHierarhy(&show_scenehier, scene);

        static bool show_objexplayout = true;
        ShowObjectInspector(&show_objexplayout, scene.GetObject(id));

        static bool show_fps = true;
        ShowFPS(&show_fps);

        ImGui::ShowDemoWindow();
        GetUI().RenderDrawData();
        
        GetGraphics().End();

        GetTimer().Sync();
    }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;

}