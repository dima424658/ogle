#include "engine.hpp"

#include <iostream>
#include <cmath>
#include <future>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 cameraView;
glm::mat4 cameraProjection;
glm::vec3 cameraPosition;
float sensivity = 0.5f;
float distance = 6.0f;

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

    cameraProjection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)900, 0.1f, 100.0f);
    cameraPosition = glm::normalize(cameraPosition) * distance;
    cameraView = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f ,1.0f ,0.0f));
}

int main()
{
    try
    {
    GetGraphics();
    Clock clock;
    clock.SetDelay(std::chrono::milliseconds(10));
    Graphics::CTexture texture("data/clock.png");
    Graphics::CTexture textureSpec("data/container2.png");
    CMesh mesh("data/clock.obj");
    Graphics::UI ui(GetGraphics().GetWindow());
    Graphics::CDeferred def(1600, 900);
    
    Sound::CSound2D::RegisterScript();
    Sound::CSound3D::RegisterScript();
        
    Script::CScript script("script.as");

    script.PrepareFunction("float calc(float, float)");
    script.GetContext()->SetArgFloat(0, 6.0f);
    script.GetContext()->SetArgFloat(1, 2.0f);
    void* result = script.Execute();

    float a = *(static_cast<float*>(result));
    System::Log() << "Result is " << a;
   // sound.Play();

    Sound::CSoundListener listener;

    int factor = 1;

    while(true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ui.ProcessEvent(event);
            if (event.type == SDL_QUIT)
                std::exit(EXIT_SUCCESS);
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) //&& event.window.windowID == SDL_GetWindowID(m_window))
                std::exit(EXIT_SUCCESS);
        }

        GetGraphics().Begin();
        cameraUpdate();
        static float delta = 0.0f;

        delta += 0.1f * factor;
        if(delta > 5)
            factor = -1;

        if(delta < -5)
            factor = 1;

        listener.Update(60, {delta, 1.0f, 0.0f});
        GetSound().Update();

        def.Begin();

        def.GetShader().Set("texture_diffuse1", 0);
        texture.Use(GL_TEXTURE0);
        def.GetShader().Set("texture_specular1", 1);
        textureSpec.Use(GL_TEXTURE1);
        
        def.GetShader().Set("projection", cameraProjection);
        def.GetShader().Set("view", cameraView);
        def.GetShader().Set("model", glm::mat4(1.0f));
        mesh.Draw();

        def.End(cameraPosition);

        ui.NewFrame(clock.GetDelta());
        static bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ui.RenderDrawData();
        
        GetGraphics().End();

        clock.Sync();
    }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;

}