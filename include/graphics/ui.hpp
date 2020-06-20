#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <graphics/graphics_device.hpp>
#include <graphics/shader.hpp>
#include <imgui.h>

namespace Graphics
{
    class UI
    {
    public:
        UI(SDL_Window* window);
        ~UI();
        
        void NewFrame(float deltaTime);
        void RenderDrawData();
        bool ProcessEvent(const SDL_Event& event);

    private:
        void InitSDL2();
        void InitOGL();
        bool CreateDeviceObjects();
        bool CreateFontsTexture();

        void SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height, GLuint vertex_array_object);

        void UpdateMousePosAndButtons();
        void UpdateMouseCursor();
        void UpdateGamepads();

        static const char* GetClipboardText(void* user_data);
        static void SetClipboardText(void* user_data, const char* text);

    private:
        SDL_Window* m_window;
        SDL_Cursor* m_mouseCursors[ImGuiMouseCursor_COUNT];
        bool m_mouseCanUseGlobalState = true;
        char* m_clipboardTextData = nullptr;
        bool m_mousePressed[3] = { false, false, false };

    //OGL
        GLuint      m_fontTexture;
        CShader*    m_shader;
        int         m_attribLocationTex, m_attribLocationProjMtx;                                // Uniforms location
        int         m_attribLocationVtxPos, m_attribLocationVtxUV, m_attribLocationVtxColor; // Vertex attributes location
        GLuint      m_vboHandle, m_elementsHandle;
    };
};