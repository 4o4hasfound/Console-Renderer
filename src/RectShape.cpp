#include "RectShape.hpp"

Rect::Rect(uint32_t width, uint32_t height) :
	m_width(width), m_height(height) {

}

void Rect::draw(WindowBase* window) const {
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			window->setPixel(x + m_position.x, y + m_position.y, m_color);
		}
	}
}
