#include "engine.hpp"

#include <iostream>
#include <cmath>
#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <functional>

#include <system/extensions.hpp>


glm::mat4 cameraView;
glm::mat4 cameraProjection;
glm::vec3 cameraPosition;
glm::vec2 cameraSize = { 1600, 900 };

float sensivity = 0.5f;
float distance = 15.0f;

void cameraUpdate()
{
    int x, y;
    //SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);
    float fx = static_cast<float>(x) / sensivity;
    float fy = static_cast<float>(y) / sensivity;

    cameraPosition.x = cos(glm::radians(fx)) * cos(glm::radians(fy));
    cameraPosition.y = sin(glm::radians(fy));
    cameraPosition.z = sin(glm::radians(fx)) * cos(glm::radians(fy));

    cameraProjection = glm::perspective(glm::radians(45.0f), cameraSize.x / cameraSize.y, 0.1f, 100.0f);
    cameraPosition = glm::normalize(cameraPosition) * distance;
    cameraView = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f ,1.0f ,0.0f));
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

bool ShowExplorer(const std::string& name, std::function<void(const std::string&)> openCb)
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

void ShowLayout(bool* p_open, CScene& scene)
{
    static uint32_t selected = 0;
    static char buffer[128];

    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Simple layout", p_open))
    {
        // Left

        ImGui::BeginChild("left pane", ImVec2(0, 400), true);
        for (auto& o : scene.GetObjects())
        {
            if (ImGui::Selectable((o.GetName() + "##" + std::to_string(o.GetID())).c_str(), selected == o.GetID()))
                selected = o.GetID();
        }
        ImGui::EndChild();

        if (ImGui::Button("Add")) 
        {
            scene.AddObject("default", true);
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete"))
        {
            scene.DeleteObject(selected);
        }

        CObject* o = scene.GetObject(selected);

        // Right
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        ImGui::Text("MyObject: %u", selected);
        ImGui::Separator();
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Description"))
            {
                if(o == nullptr)
                    ImGui::TextWrapped("Please select an object.");
                else
                {
                    strncpy_s(buffer, o->GetName().c_str(), 128);
                    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
                        o->SetName(std::string(buffer));

                    ImGui::DragFloat3("Position", glm::value_ptr(o->GetPosition()), 0.05f, 0.0f, 0.0f, "%.3f");

                    if (o->GetSound3D())
                        if (ImGui::CollapsingHeader("Sound3D", ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            ImGui::Text("Sound name: %s", o->GetSound3D()->GetName().c_str());

                            bool playing = o->GetSound3D()->IsPlaying();
                            if (ImGui::Checkbox("Playing", &playing))
                                playing ? o->GetSound3D()->Play() : o->GetSound3D()->Pause();

                            float volume = o->GetSound3D()->GetVolume();

                            if (ImGui::DragFloat("Volume", &volume, 0.1f, 0.0f, 100.0f, "%.1f"))
                                o->GetSound3D()->SetVolume(volume);
                        }

                    if (o->GetMesh())
                        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            static char mesh_buffer[128] = { '\0' };

                            if (o->GetMesh()->isLoaded())
                            {
                                ImGui::Text("Object name: %s", o->GetMesh()->GetName().c_str());
                            }

                            if (ImGui::Button("Load Model.."))
                                ImGui::OpenPopup("load_model_popup");

                            if (ImGui::BeginPopup("load_model_popup"))
                            {
                                ImGui::InputText("Name", mesh_buffer, IM_ARRAYSIZE(mesh_buffer));
                                ImGui::SameLine();
                                if (ImGui::Button("..."))
                                {
                                    GetUI().CreateWindow("asd", std::bind(&ShowExplorer, std::placeholders::_1, [o](const std::string& path)
                                        {
                                            strncpy_s(mesh_buffer, path.c_str(), 128);
                                            o->GetMesh()->LoadModel(mesh_buffer);
                                        }));
                                }

                                if (ImGui::Button("Load"))
                                    o->GetMesh()->LoadModel(mesh_buffer);

                                ImGui::EndPopup();
                            }
                        }

                    if(o->GetTexture())
                        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            ImGui::BeginChild("texture_child");
                            static char texture_buffer[128] = { '\0' };

                            if (ImGui::Button("Load Texture.."))
                                ImGui::OpenPopup("load_texture_popup");

                            if (ImGui::CollapsingHeader("Texture preview"))
                                ImGui::Image((void*)(intptr_t)o->GetTexture()->GetID(), ImVec2(ImGui::GetWindowWidth() / 1.5, ImGui::GetWindowWidth() / 1.5));

                            if (ImGui::BeginPopup("load_texture_popup"))
                            {
                                ImGui::InputText("Name", texture_buffer, IM_ARRAYSIZE(texture_buffer));
                                if (ImGui::Button("Load"))
                                    o->GetTexture()->LoadImage(texture_buffer);

                                ImGui::EndPopup();
                            }
                            ImGui::EndChild();
                        }
                }

                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("Details"))
            {
                ImGui::Text("ID: %u", selected);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();

        if (o)
        {
            const char* component_names[] = { "Sound3D", "Mesh", "Texture" };

            if (ImGui::Button("Add component.."))
                ImGui::OpenPopup("component_popup");

            if (ImGui::BeginPopup("component_popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(component_names); i++)
                    if (ImGui::Selectable(component_names[i]))
                    {
                        if(i == 0 && !o->GetSound3D())
                                o->CreateSound3D(new Sound::CSound3D("data/test.mp3", false));

                        if (i == 1 && !o->GetMesh())
                            o->CreateMesh(new Graphics::CMesh());

                        if (i == 2 && !o->GetTexture())
                            o->CreateTexture(new Graphics::CTexture());
                    }
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
        }
    }

    ImGui::End();
}

int main()
{
    try
    {
    GetGraphics();
    GetTimer();
    CScene scene;
    Graphics::CTexture texture("data/clock.png");
    Graphics::CMesh mesh("data/clock.obj");
    Graphics::CTexture textureSpec("data/container2.png");
    
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

    int factor = 1;

    GetTimer().Sync();

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
                    
                }
            }
        }

        GetGraphics().Begin();
        cameraUpdate();
        static float delta = 0.0f;

        delta += 0.1f * factor;
        if(delta > 5)
            factor = -1;

        if(delta < -5)
            factor = 1;

        listener.Update(GetTimer().GetDelta() * 1000, toFMOD_VECTOR(cameraPosition));
        GetSound().Update();

        def.Prepare();

        def.GetShader().Set("texture_diffuse1", 0);
        texture.Use(GL_TEXTURE0);
        def.GetShader().Set("texture_specular1", 1);
        textureSpec.Use(GL_TEXTURE1);
        
        def.GetShader().Set("projection", cameraProjection);
        def.GetShader().Set("view", cameraView);
        def.GetShader().Set("model", glm::mat4(1.0f));
        mesh.Draw();

        scene.Update();

        def.Draw(cameraPosition);

        GetUI().NewFrame(GetTimer().GetDelta());
        static bool show_demo_window = true;
        ShowExampleAppDockSpace(&show_demo_window);

        static bool show_layout = true;
        ShowLayout(&show_layout, scene);

       
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