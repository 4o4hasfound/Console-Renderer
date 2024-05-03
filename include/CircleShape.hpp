#ifndef CIRCLE_SHAPE_INCLUDED
#define CIRCLE_SHAPE_INCLUDED

#include<unordered_map>
#include<vector>

#include "WindowBase.hpp"
#include "Math/Vector.hpp"

class Circle : public Drawable {
public:
	Circle() = default;
	Circle(uint32_t radius);

	void setRadius(float radius);
	const float& getRadius() const { return m_radius; };

	void setPosition(const ivec2& position) { m_position = position; };
	const ivec2& getPosition() const { return m_position; };

	void setColor(const ivec3& color) { m_color = color; };
	const ivec3& getColor() const { return m_color; };
private:
	virtual void draw(WindowBase* window) override;

	float m_radius;
	ivec2 m_position;
	ivec3 m_color;

	std::vector<ivec2>* m_vertices;

	static std::unordered_map<uint32_t, std::vector<ivec2>* > vertices_map;
};


#endif