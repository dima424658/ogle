#pragma once

#include <tuple>
#include <cstdint>
#include <SDL2/SDL.h>

namespace System
{
    class CInput
    {
    private:
        CInput();
        CInput(const CInput&) = delete;  
        CInput& operator=(CInput&) = delete;
    public:
        ~CInput();

        void Update();

        const std::pair<int, int>& GetMousePosition() const;
        const std::pair<int, int>& GetMouseDelta() const;

        bool IsKeyPressed(size_t key) const;

        bool IsMousePressed(uint32_t button) const;

        static CInput& Instance();

    private:
        const uint8_t* m_keyboardState;
        uint32_t m_mouseState;
        std::pair<int, int> m_mouse;
        std::pair<int, int> m_mousePrev;
        std::pair<int, int> m_mouseDelta;

    };

    inline CInput& GetInput()
    {
        return CInput::Instance();
    }
};