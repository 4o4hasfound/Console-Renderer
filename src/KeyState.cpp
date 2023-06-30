#include "KeyState.hpp"

VirtualKeyState KeyState::m_keys[256], KeyState::m_mouse[5];
VirtualKeyState KeyState::m_lastkeys[256], KeyState::m_lastmouse[5];

ivec2 KeyState::m_mousePos = ivec2(0);
ivec2 KeyState::m_lastMousePos = ivec2(0);

int16_t KeyState::m_keyOldState[256] = {0};
int16_t KeyState::m_keyNewState[256] = { 0 };
bool KeyState::m_mouseOldState[5] = {0};
bool KeyState::m_mouseNewState[5] = {0};

void KeyState::update() {
	std::copy(m_keys, m_keys + 256, m_lastkeys);
	std::copy(m_mouse, m_mouse + 5, m_lastmouse);
	m_lastMousePos = m_mousePos;

	for (int i = 0; i < 256; i++)
	{
		m_keyNewState[i] = GetAsyncKeyState(i);

		m_keys[i].bPressed = false;
		m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
			if (m_keyNewState[i] & 0x8000)
			{
				m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
			}
			else
			{
				m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
			}
		}

		m_keyOldState[i] = m_keyNewState[i];
	}

	INPUT_RECORD inBuf[32];
	DWORD events = 0;
	GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &events);
	if (events > 0)
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inBuf, events, &events);

	for (DWORD i = 0; i < events; i++) 
	{
		switch (inBuf[i].EventType) {
		case MOUSE_EVENT:
			switch (inBuf[i].Event.MouseEvent.dwEventFlags) {
				case MOUSE_MOVED:
					{
						m_mousePos.x = inBuf[i].Event.MouseEvent.dwMousePosition.X;
						m_mousePos.y = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
					}
					break;

				case 0:
					{
						for (int m = 0; m < 5; m++)
							m_mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;

					}
					break;

			}

		}
	}

	for (int m = 0; m < 5; m++) {
		m_mouse[m].bPressed = false;
		m_mouse[m].bReleased = false;

		if (m_mouseNewState[m] != m_mouseOldState[m])
		{
			if (m_mouseNewState[m])
			{
				m_mouse[m].bPressed = true;
				m_mouse[m].bReleased = false;
				m_mouse[m].bHeld = true;
			}
			else
			{
				m_mouse[m].bPressed = false;
				m_mouse[m].bReleased = true;
				m_mouse[m].bHeld = false;
			}
		}

		m_mouseOldState[m] = m_mouseNewState[m];
	}
}
