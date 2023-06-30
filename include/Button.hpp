#ifndef BUTTON_INCLUDED
#define BUTTON_INCLUDED

#include<functional>
#include "Math/Vector.hpp"
#include "Window.hpp"
#include "CallBack.hpp"
#include "Gadget.hpp"
#include "KeyState.hpp"

class WindowBase;

class Button: public Gadget {
public:
	Button(uint32_t width, uint32_t height);

	virtual void update(WindowBase* window) override;
	virtual void render(WindowBase* window) override;

	void setPosition(const ivec2& position) { m_position = position; }
	constexpr ivec2 getPosition() const { return m_position; }

	void setColor(const ivec3& color) { m_color = color; }
	constexpr ivec3 getColor() const { return m_color; }

	constexpr uint32_t getWidth() const { return m_width; }
	constexpr uint32_t getHeight() const { return m_height; }
private:
	uint32_t m_width, m_height;
	ivec2 m_position;
	ivec3 m_color;
};

#endif