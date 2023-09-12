#include "Text.hpp"

Text::Text(Font* font): m_font(font) {
    this->setScale(m_pixelScale);
}

void Text::setString(const std::string& text) {
    if (m_string != text) reRenderBitmap = true;
    m_string = text;
}

void Text::setScale(const float scale) {
    if (m_pixelScale != scale) reRenderBitmap = true;
    m_pixelScale = scale;
    int ascent;
    m_scale = stbtt_ScaleForPixelHeight(m_font->getFont(), m_pixelScale);
    stbtt_GetFontVMetrics(m_font->getFont(), &ascent, 0, 0);
    m_baseline = (int)(ascent * m_scale);
}

void Text::setFont(Font* font) {
    if (m_font != font) reRenderBitmap = true;
    m_font = font;
}

void Text::setSpace(int space) {
    if (m_space != space) reRenderBitmap = true;
    m_space = space;
}

void Text::draw(WindowBase* window) {
    if (reRenderBitmap) {
        reRenderBitmap = false;
        this->RenderBitnmap();
    }
    for (int y = 0; y < m_bitmap.size(); ++y) {
#	pragma omp parallel for
        for (int x = 0; x < m_bitmap[y].size(); ++x) {
            if (m_bitmap[y][x])
                window->setPixel(x + m_position.x, y + m_position.y, m_color);
        }
    }
}

void Text::RenderBitnmap() {
    int xpos = 0;
    unsigned int exproximateW = (float)m_string.size() * m_pixelScale / 2;
    unsigned char* screen = new unsigned char[(int)m_pixelScale * exproximateW]();
    m_bitmap.resize(m_pixelScale);
    for (int j = 0; j < m_bitmap.size(); ++j) m_bitmap[j].resize(exproximateW);
    for (int i = 0; i < m_string.size(); ++i) {
        int advance, lsb, x0, y0, x1, y1;
        float x_shift = xpos - (float)floor(xpos);
        stbtt_GetCodepointHMetrics(m_font->getFont(), m_string[i], &advance, &lsb);
        stbtt_GetCodepointBitmapBoxSubpixel(m_font->getFont(), m_string[i], m_scale, m_scale, x_shift, 0, &x0, &y0, &x1, &y1);
        stbtt_MakeCodepointBitmapSubpixel(m_font->getFont(), &screen[(m_baseline + y0) * exproximateW + (int)xpos + x0], x1 - x0, y1 - y0, exproximateW, m_scale, m_scale, x_shift, 0, m_string[i]);
        xpos += (advance * m_scale);
        if (i != m_string.size() - 1) {
            xpos += m_scale * stbtt_GetCodepointKernAdvance(m_font->getFont(), m_string[i], m_string[i + 1]) + m_space;
        }
    }
    for (int y = 0; y < m_pixelScale; ++y) {
        for (int x = 0; x < exproximateW; ++x) {
            m_bitmap[y][x] = screen[y * exproximateW + x];
        }
    }
    delete[] screen;
}

