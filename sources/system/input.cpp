#include "system/input.hpp"

using namespace System;

CInput::CInput()
{
    m_keyboardState = SDL_GetKeyboardState(nullptr);
    m_mouseState = SDL_GetMouseState(&m_mouse.first, &m_mouse.second);
    m_mousePrev = m_mouse;
    m_mouseDelta = {0, 0};
}

CInput::~CInput()
{

}

CInput& CInput::Instance()
{
    static CInput input;
    return input;
}

void CInput::Update()
{
    m_mousePrev = m_mouse;

    m_keyboardState = SDL_GetKeyboardState(nullptr);
    m_mouseState = SDL_GetMouseState(&m_mouse.first, &m_mouse.second);
    m_mouseDelta = {m_mouse.first - m_mousePrev.first, m_mouse.second - m_mousePrev.second};
}

const std::pair<int, int>& CInput::GetMousePosition() const
{
    return m_mouse;
}

const std::pair<int, int>& CInput::GetMouseDelta() const
{
    return m_mouseDelta;
}

bool CInput::IsKeyPressed(size_t key) const
{    
    return m_keyboardState[key];
}

bool CInput::IsMousePressed(uint32_t button) const
{
    return m_mouseState & SDL_BUTTON(button);
}