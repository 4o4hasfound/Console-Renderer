#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

#include <string>
#include <vector>

#include "Font.hpp"
#include "Drawable.hpp"
#include "Window.hpp"
#include "Math/Vector.hpp"
//#include "stb_truetype.h"

class Text: public Drawable {
public:
	Text(Font* font);
	~Text() = default;

	void setString(const std::string& text);
	const std::string& getString() const { return m_string; }
	void setPosition(const ivec2& pos) { m_position = pos; }
	const ivec2& getPosition() const { return m_position; }
	void setScale(const float scale);
	const float getScale() const { return m_pixelScale; }
	void setColor(const ivec3& color) { m_color = color; }
	const ivec3& getColor() const { return m_color; }
	void setFont(Font* font);
	const Font* getFont() const { return m_font; }
	void setSpace(int space);
	int getSpace() const { return m_space; }

private:
	virtual void draw(WindowBase* window) override;

	void RenderBitnmap();

	Font* m_font;

	std::string m_string;
	ivec2 m_position = ivec2(0);
	ivec3 m_color = ivec3(255);
	float m_pixelScale = 20.0;
	int m_space = 0;
	float m_scale;
	int m_baseline;

	bool reRenderBitmap = false;

	std::vector<std::vector<unsigned char> > m_bitmap;
};


#endif