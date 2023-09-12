#include "Font.hpp"

Font::Font(const std::string& ttfpath) {
    unsigned char* buffer = new unsigned char[24 << 20];;

    fread(buffer, 1, 1000000, fopen(ttfpath.c_str(), "rb"));
    stbtt_InitFont(&m_font, buffer, 0);
}
