#ifndef FONT_INCLUDED
#define FONT_INCLUDED

#include <stdio.h>
#include <cmath>
#include <string>
#include "stb_truetype.h"

class Font {
public:
	Font(const std::string& ttfpath);
	const stbtt_fontinfo* getFont() const { return &m_font; }
private:
	stbtt_fontinfo m_font;
};


#endif