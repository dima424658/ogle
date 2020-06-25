#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <graphics/device.hpp>
#include <graphics/shader.hpp>
#include <imgui.h>

#include <list>

#undef CreateWindow

namespace Graphics
{
    class UI
    {
        UI();
        UI(const UI&) = delete;
        UI& operator=(UI&) = delete;
    public:
        // returns true if needs to be closed
        typedef std::function<bool(const std::string& name)> window_cb;

        ~UI();
        
        void NewFrame(float deltaTime);
        void RenderDrawData();
        bool ProcessEvent(const SDL_Event& event);

        static UI& Instance();

        void CreateWindow(std::string name, window_cb callback);

    private:
        void UpdateWindows();

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
        std::list<std::pair<std::string, window_cb>> m_windows;

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