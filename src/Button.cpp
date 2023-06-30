#include "Button.hpp"

Button::Button(uint32_t width, uint32_t height) :
	m_width(width), m_height(height) {

}

void Button::update(WindowBase* window) {
	const ivec2 pos = KeyState::getMousePos(window);
	if (pos.x >= m_position.x && pos.x <= m_position.x + m_width &&
		pos.y >= m_position.y && pos.y <= m_position.y + m_height) {
		m_callback->onHover();
		if (KeyState::getKey(VK_LBUTTON).bPressed) {
			m_callback->onLClick();
			m_dragged = true;
		}
	}
	else {
		m_callback->onNotHover();
	}
	if (!KeyState::getKey(VK_LBUTTON).bHeld) {
		m_dragged = false;
	}
	if (m_dragged) {
		const ivec2 now = KeyState::getMousePos(window);
		const ivec2 last = KeyState::getLastMousePos(window);
		m_callback->onDrag(now - last);
	}
}

void Button::render(WindowBase* window) {
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			window->setPixel(x + m_position.x, y + m_position.y, m_color);
		}
	}
}
