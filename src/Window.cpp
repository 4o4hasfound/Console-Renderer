#include "Window.hpp"

std::unordered_map<int8_t, uint64_t> Window::ToRGB = {
	   {BG_BLACK, RGB(0, 0, 0)},
	   {BG_DARK_BLUE, RGB(0, 0, 128)},
	   {BG_DARK_GREEN, RGB(0, 128, 0)},
	   {BG_DARK_CYAN, RGB(0, 128, 128)},
	   {BG_DARK_RED, RGB(128, 0, 0)},
	   {BG_DARK_MAGENTA, RGB(128, 0, 0)},
	   {BG_DARK_YELLOW, RGB(128, 128, 0)},
	   {BG_GREY, RGB(192, 192, 192)},
	   {BG_DARK_GREY, RGB(128, 128, 128)},
	   {BG_BLUE, RGB(0, 0, 255)},
	   {BG_GREEN, RGB(0, 255, 0)},
	   {BG_CYAN, RGB(0, 255, 255)},
	   {BG_RED, RGB(255, 0, 0)},
	   {BG_MAGENTA, RGB(255, 0, 255)},
	   {BG_YELLOW, RGB(255, 255, 0)},
	   {BG_WHITE, RGB(255, 255, 255)},
	   {FG_BLACK, RGB(0, 0, 0)},
	   {FG_DARK_BLUE, RGB(0, 0, 128)},
	   {FG_DARK_GREEN, RGB(0, 128, 0)},
	   {FG_DARK_CYAN, RGB(0, 128, 128)},
	   {FG_DARK_RED, RGB(128, 0, 0)},
	   {FG_DARK_MAGENTA, RGB(128, 0, 0)},
	   {FG_DARK_YELLOW, RGB(128, 128, 0)},
	   {FG_GREY, RGB(192, 192, 192)},
	   {FG_DARK_GREY, RGB(128, 128, 128)},
	   {FG_BLUE, RGB(0, 0, 255)},
	   {FG_GREEN, RGB(0, 255, 0)},
	   {FG_CYAN, RGB(0, 255, 255)},
	   {FG_RED, RGB(255, 0, 0)},
	   {FG_MAGENTA, RGB(255, 0, 255)},
	   {FG_YELLOW, RGB(255, 255, 0)},
	   {FG_WHITE, RGB(255, 255, 255)}
};;

// RGB value to CHAR_INFO
std::unordered_map<int32_t, CHAR_INFO> Window::ToCharInfo = std::unordered_map<int32_t, CHAR_INFO>();

Window::Window(uint32_t width, uint32_t height, uint32_t characterSize) {
	this->setupWindow(width, height, characterSize);
	this->SetUpRGBMap();
}

void Window::setTitle(std::wstring title) {
	SetConsoleTitle(title.c_str());
}

void Window::setTitle(std::string title) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(title.c_str());
	this->setTitle(wide);
}

void Window::setPixel(uint32_t x, uint32_t y, const ivec3& color) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return;
	}
	CHAR_INFO result = ToCharInfo[RGB(
		color.x - color.x % ColorMapStep,
		color.y - color.y % ColorMapStep,
		color.z - color.z % ColorMapStep
	)];
	m_colorbuffer[static_cast<uint32_t>(y) * m_width + x] = result;
}

void Window::setPixel(uint32_t x, uint32_t y, CHAR_INFO char_info) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return;
	}
	m_colorbuffer[static_cast<uint32_t>(y) * m_width + x] = char_info;
}

void Window::fill(const ivec3& color) {
	//#	pragma omp parallel for
	for (int i = 0; i < m_buffersize; ++i) {
		m_colorbuffer[i].Char.UnicodeChar = PIXEL_SOLID;
		m_colorbuffer[i].Attributes = 0;
	}
}

void Window::render() {
	// Render Stuff
	const COORD buffersize = {
		m_width,
		m_height
	};
	SMALL_RECT rectWindow = { 0, 0, (short)m_width - 1, (short)m_height - 1 };

	WriteConsoleOutput(m_stdout_handle, m_colorbuffer, buffersize, { 0, 0 }, &rectWindow);
	
}

void Window::SetUpRGBMap() {
	if (!ToCharInfo.empty())
		return;
	auto dis = [](uint64_t c1, uint64_t c2) {
		return	abs(GetRValue(c1) - GetRValue(c2)) /** (GetRValue(c1) - GetRValue(c2)) */+
			abs(GetGValue(c1) - GetGValue(c2)) /** (GetGValue(c1) - GetGValue(c2))*/ +
			abs(GetBValue(c1) - GetBValue(c2))/* * (GetBValue(c1) - GetBValue(c2))*/;
	};
	// Loop over every RGB value with a step of ColorMapStep
	for (int r = 0; r < 256; r += ColorMapStep) {
		for (int g = 0; g < 256; g += ColorMapStep) {
			for (int b = 0; b < 256; b += ColorMapStep) {
				CHAR_INFO info;
				info.Char.UnicodeChar = PIXEL_SOLID;
				info.Attributes = 0;
				uint64_t outColor = 0U;
				uint64_t target = RGB(r, g, b);
				for (int fg = 0; fg <= 15; ++fg) { // FG
					for (int bg = 0; bg <= 240; bg += 16) { // BG
						for (int k = 0; k < 4; ++k) { // Pixel
							uint64_t nowColor;
							WORD nowChar;
							const float tquarterCoverage = 0.7;	// Magic Number go brrrrrr
							const float halfCoverage = 0.5;		// Magic Number go brrrrrr
							const float quarterCoverage = 0.3;	// Magic Number go brrrrrr

							// Determine the Character type and the fg bg color proportion
							switch (k) {
							case 0:
								nowColor = ToRGB[fg];
								nowChar = PIXEL_SOLID;
								break;
							case 1:
								nowColor = ToRGB[fg] * tquarterCoverage + ToRGB[bg] * (1- tquarterCoverage);
								nowChar = PIXEL_THREEQUARTERS;
								break;
							case 2:
								nowColor = ToRGB[fg] * halfCoverage + ToRGB[bg] * (1- halfCoverage);
								nowChar = PIXEL_HALF;
								break;
							case 3:
								nowColor = ToRGB[fg] * quarterCoverage + ToRGB[bg] * (1- quarterCoverage);
								nowChar = PIXEL_QUARTER;
								break;
							}
							if (dis(nowColor, target) < dis(outColor, target)) {
								outColor = nowColor;
								info.Attributes = fg | bg;
								info.Char.UnicodeChar = nowChar;
							}
						}
					}
				}
				ToCharInfo[target] = info;
			}
		}
	}
}

