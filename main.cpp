#include "engine.hpp"

#include <iostream>
#include <cmath>
#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <filesystem>
#include <functional>

#include <platform/info.hpp>
#include <platform/linux.hpp>

int main(int argc, char **argv)
{
    try 
    {
        Graphics::GetGraphics();
        System::GetTimer().SetDelay(10);

        CScene scene;
        scene.LoadScene("./scene.json");

        Graphics::CDeferred def(1600, 900);
        Graphics::CEditor editor(scene);
        
        RegisterStdString(Script::GetScript().GetEngine());

        Script::RegisterGLM();
        Script::RegisterFunctions(Script::GetScript().GetEngine());

        Sound::CSound2D::RegisterScript();
        Sound::CSound3D::RegisterScript();

        Sound::CSoundListener listener;

        int width, height;
        SDL_GetWindowSize(Graphics::GetGraphics().GetWindow(), &width, &height);

        System::GetTimer().Sync();
        System::GetInput();

        while (true)
        {
            SDL_PumpEvents();
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                Graphics::GetUI().ProcessEvent(event);
                if (event.type == SDL_QUIT)
                    std::exit(EXIT_SUCCESS);
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) //&& event.window.windowID == SDL_GetWindowID(m_window))
                    std::exit(EXIT_SUCCESS);
            }

            listener.Update(System::GetTimer().GetDelta(), toFMOD_VECTOR(scene.GetActiveCamera().GetPosition()),
             toFMOD_VECTOR(glm::normalize(glm::cross(scene.GetActiveCamera().GetRight(), scene.GetActiveCamera().GetUp()))),
             toFMOD_VECTOR(glm::normalize(scene.GetActiveCamera().GetUp())));

            Sound::GetSound().Update();


            Graphics::GetGraphics().Begin();

            Graphics::GetUI().NewFrame(System::GetTimer().GetDelta() / 1000);

            editor.Update();

            scene.Update();
            scene.Draw(def);

        static bool showd = true;

            ImGui::ShowDemoWindow(&showd);
            
            Graphics::GetUI().RenderDrawData();

            Graphics::GetGraphics().End();

            System::GetTimer().Sync();
            System::GetInput().Update();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}