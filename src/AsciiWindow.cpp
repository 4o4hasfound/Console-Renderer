#include "AsciiWindow.hpp"

AsciiWindow::AsciiWindow(uint32_t width, uint32_t height, uint32_t characterSize, std::string ascii) :
	m_ascii(ascii) {
	this->setupWindow(width, height, characterSize);
	this->sortAscii();
}

void AsciiWindow::sortAscii() {
	Font font("C:/windows/fonts/cour.ttf");

	unsigned char* bitmap;
	int w, h, pixels;
	const int pixelSize = 50;

	std::vector<std::pair<char, float> > tmp(m_ascii.size());

	for (int i = 0; i < m_ascii.size(); ++i) {
		tmp[i].first = m_ascii[i];
		bitmap = stbtt_GetCodepointBitmap(font.getFont(), 0, stbtt_ScaleForPixelHeight(font.getFont(), pixelSize), m_ascii[i], &w, &h, 0, 0);

		pixels = 0;
		for (int j = 0; j < h; ++j) {
			for (int k = 0; k < w; ++k) {
				if (bitmap[j * w + k] > 0) ++pixels;
			}
		}
		tmp[i].second = static_cast<float>(pixels) / (pixelSize * pixelSize);
		free(bitmap);
	}

	std::sort(tmp.begin(), tmp.end(), 
		[](const std::pair<char, float>& lhs, const std::pair<char, float>& rhs) {
			return lhs.second < rhs.second;
		}
	);

	for (int i = 0; i < tmp.size(); ++i) {
		tmp[i].second /= tmp.back().second;
	}

	for (int i = 0; i < m_ascii.size(); ++i) {
		m_ascii[i] = tmp[i].first;
	}

	for (int i = 0, asciiPtr = 0, count = 0; i < 256; ++i) {
		float idealCoverage = static_cast<float>(i) / 255.0;
		while (asciiPtr < m_ascii.size() - 1 &&
			std::abs(tmp[asciiPtr].second - idealCoverage) > std::abs(tmp[asciiPtr + 1].second - idealCoverage)) {
			++asciiPtr;
			count = 0;
		}
		if (++count > 2 && asciiPtr < m_ascii.size() - 1) {
			++asciiPtr;
			count = 0;
		}
		m_pallet[i] = m_ascii[asciiPtr];
	}

	int x = 1;
}

void AsciiWindow::setTitle(std::wstring title) {
	SetConsoleTitle(title.c_str());
}

void AsciiWindow::setTitle(std::string title) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(title.c_str());
	this->setTitle(wide);
}

void AsciiWindow::setPixel(uint32_t x, uint32_t y, const ivec3& color) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return;
	}
	m_colorbuffer[static_cast<uint32_t>(y) * m_width + x] = this->RGBToAscii(color);
}

void AsciiWindow::setPixel(uint32_t x, uint32_t y, CHAR_INFO char_info) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return;
	}
	m_colorbuffer[static_cast<uint32_t>(y) * m_width + x] = char_info;
}

void AsciiWindow::fill(const ivec3& color) {
	//#	pragma omp parallel for
	for (int i = 0; i < m_buffersize; ++i) {
		m_colorbuffer[i].Char.UnicodeChar = PIXEL_SOLID;
		m_colorbuffer[i].Attributes = 0;
	}
}

void AsciiWindow::render() {
	// Render Stuff
	const COORD buffersize = {
		m_width,
		m_height
	};
	SMALL_RECT rectWindow = { 0, 0, (short)m_width - 1, (short)m_height - 1 };

	WriteConsoleOutput(m_stdout_handle, m_colorbuffer, buffersize, { 0, 0 }, &rectWindow);

}

CHAR_INFO AsciiWindow::RGBToAscii(const ivec3& color) {
	CHAR_INFO ret;

	float grayscale = 0.2989 * color.r + 0.5870 * color.g + 0.1140 * color.b;
	assert(grayscale < 256);
	ret.Char.AsciiChar = m_pallet[static_cast<int>(grayscale)];
	ret.Attributes = FG_WHITE | BG_BLACK;

	return ret;
}
