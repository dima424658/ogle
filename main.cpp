#include "engine.hpp"

#include <iostream>
#include <cmath>
#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <filesystem>
#include <functional>

int main()
{
    try
    {
    Graphics::GetGraphics();
    System::GetTimer().SetDelay(10);
    CScene scene;
    CCamera camera;
    Graphics::CTexture texture("data/256.jpeg");
    //Graphics::CMesh mesh("data/cube.obj");
    Graphics::CTexture textureSpec("data/container2_specular.png");
    
    Graphics::CDeferred def(1600, 900);
    Graphics::CEditor editor(scene);
    
	RegisterStdString(Script::GetScript().GetEngine());

    Script::RegisterGLM();
    Script::RegisterFunctions(Script::GetScript().GetEngine());
    
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

    int width, height;
    SDL_GetWindowSize(Graphics::GetGraphics().GetWindow(), &width, &height);
    camera.SetRenderSize(width, height);
    camera.Update();

    System::GetTimer().Sync();
    System::GetInput();

    auto id = scene.AddObject("cube", true);
    scene.GetObject(id)->CreateMesh(new Graphics::CMesh("data/cube.obj"));

    id = scene.AddObject("camera", true);
    scene.GetObject(id)->CreateCamera(new CCamera());

    while(true)
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
           
        //listener.Update(GetTimer().GetDelta() * 1000, toFMOD_VECTOR(cameraPosition));
        Sound::GetSound().Update();
        
        scene.Update();

        Graphics::GetGraphics().Begin();

        Graphics::GetUI().NewFrame(System::GetTimer().GetDelta() / 1000);


        editor.Update();

        scene.Draw(def);

        Graphics::GetUI().RenderDrawData();
        
        Graphics::GetGraphics().End();

        System::GetTimer().Sync();
        System::GetInput().Update();
    }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;

}