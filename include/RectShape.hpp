#ifndef RECT_SHAPE_INCLUDED
#define RECT_SHAPE_INCLUDED

#include "Drawable.hpp"
#include "Math/Vector.hpp"

class Rect :  public Drawable {
public:
	Rect(uint32_t width, uint32_t height);

	void setPosition(const ivec2& position) { m_position = position; }
	constexpr ivec2 getPosition() const { return m_position; }

	void setColor(const ivec3& color) { m_color = color; }
	constexpr ivec3 getColor() const { return m_color; }

	constexpr uint32_t getWidth() const { return m_width; }
	constexpr uint32_t getHeight() const { return m_height; }

private:
	virtual void draw(WindowBase* window) override;

	uint32_t m_width, m_height;
	ivec2 m_position;
	ivec3 m_color;
};

#endif