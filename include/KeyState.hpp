#ifndef KEYSTATE_INCLUDED
#define KEYSTATE_INCLUDED

#define NOMINMAX
#include<Windows.h>

#include "Window.hpp"
#include "Math/Vector.hpp"

class WindowBase;

struct VirtualKeyState {
	bool bPressed = 0;
	bool bReleased = 0;
	bool bHeld = 0;
};

class KeyState {
public:
	KeyState() = delete;

	static void update();
	static VirtualKeyState getKey(uint8_t key) { return m_keys[key]; }
	static VirtualKeyState getLastKey(uint8_t key) { return m_lastkeys[key]; }
	static const ivec2& getMousePos() { return m_mousePos; }
	static const ivec2& getMousePos(WindowBase* window) { return m_mousePos - window->getWindowPos(); }
	static const ivec2& getLastMousePos() { return m_lastMousePos; }
	static const ivec2& getLastMousePos(WindowBase* window) { return m_lastMousePos - window->getWindowPos(); }

private:
	static VirtualKeyState m_keys[256], m_mouse[5];
	static VirtualKeyState m_lastkeys[256], m_lastmouse[5];

	static ivec2 m_mousePos;
	static ivec2 m_lastMousePos;

	static int16_t m_keyOldState[256];
	static int16_t m_keyNewState[256];
	static bool m_mouseOldState[5];
	static bool m_mouseNewState[5];
};


#endif