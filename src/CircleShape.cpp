#include "CircleShape.hpp"

std::unordered_map<uint32_t, std::vector<ivec2>* > Circle::vertices_map = std::unordered_map<uint32_t, std::vector<ivec2>* >();

Circle::Circle(uint32_t radius) : 
	m_radius(radius), m_position(0), m_color(0) {
	if (vertices_map.find(m_radius) == vertices_map.end()) {
		const float radSqr = m_radius * m_radius;
		vertices_map[m_radius] = new std::vector<ivec2>;
		m_vertices = vertices_map[m_radius];
		(*m_vertices).reserve(radSqr * 4);
#		pragma omp parallel for
		for (int y = -m_radius; y <= m_radius; ++y) {
#			pragma omp parallel for
			for (int x = -m_radius; x <= m_radius; ++x) {
				if (x * x + y * y <= radSqr) {
					m_vertices->emplace_back(x, y);
				}
			}
		}
	}
	else {
		m_vertices = vertices_map[m_radius];
	}
}

void Circle::draw(WindowBase* window) const {
#	pragma omp parallel for
	for (int i = 0; i < m_vertices->size(); ++i) {
		window->setPixel((*m_vertices)[i].x  + m_position.x, (*m_vertices)[i].y + m_position.y, m_color);
	}
}
