#include "WindowBase.hpp"

void WindowBase::setupWindow(uint32_t width, uint32_t height, uint32_t characterSize) {
	m_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(m_stdin_handle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(m_stdin_handle, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(m_stdin_handle, &cursorInfo);

	m_width = width;
	m_height = height;
	m_buffersize = m_width * m_height;
	m_colorbuffer = new CHAR_INFO[m_buffersize];

	for (int i = 0; i < m_buffersize; ++i) {
		m_colorbuffer[i].Char.UnicodeChar = PIXEL_SOLID;
		m_colorbuffer[i].Attributes = 0;
	}
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = characterSize;
	cfi.dwFontSize.Y = characterSize;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(m_stdout_handle, FALSE, &cfi);

	SMALL_RECT windowSize = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_stdout_handle, TRUE, &windowSize);
	COORD bufferSize = { m_width, m_height };
	SetConsoleScreenBufferSize(m_stdout_handle, bufferSize);
	SetConsoleActiveScreenBuffer(m_stdout_handle);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_stdout_handle, &csbi);

	SMALL_RECT rectWindow = { 0, 0, (short)m_width - 1, (short)m_height - 1 };
	SetConsoleWindowInfo(m_stdout_handle, TRUE, &rectWindow);
	m_console = GetConsoleWindow();
}

void WindowBase::draw(Drawable* object) {
	object->draw(this);
}

void WindowBase::addGadget(Gadget* gadget) {
	m_gadgets.push_back(gadget);
}

void WindowBase::updateGadgets() {
	// Update Gadgets
	for (int i = 0; i < m_gadgets.size(); ++i) {
		if (m_gadgets[i]->alive)
			m_gadgets[i]->update(this);
	}
}

void WindowBase::renderGadgets() {
	// Render Gadgets
	for (int i = 0; i < m_gadgets.size(); ++i) {
		if (m_gadgets[i]->alive)
			this->draw(m_gadgets[i]);
	}
}
